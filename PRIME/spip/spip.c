//
// Copyright (c) 2020, Tarlogic Security SL
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of copyright holders nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include <string.h>
#include <stdarg.h>

#ifdef __SAM4CMS16C_0__
#  include <lcd.h>
#endif /* __SAM4CMS16C_0__ */

#include "spip.h"

#include "util/util.h"

static void *
spip_ctx_assert_alloc(spip_ctx_t *self, size_t size)
{
  void *newbuf;
  void *ret = NULL;

  if (self->alloc_size < size) {
    TRY(newbuf = realloc(self->alloc_buf, size));
    self->alloc_buf = newbuf;
    self->alloc_size = size;
  }

  ret = self->alloc_buf;

fail:
  return ret;
}

static void
spip_ctx_resync(spip_ctx_t *self)
{
  memset(self->sync, 0, 8);
}

static BOOL
spip_ctx_sync(spip_ctx_t *self, uint8_t c)
{
  uint8_t p, q;
  union {
    uint8_t sorted[8];
    uint64_t sync;
  } syncbuf;


  self->sync[self->p++] = c;
  if (self->p == 8)
    self->p = 0;

  p = self->p;

  for (q = 0; q < 8; ++q) {
    syncbuf.sorted[q] = self->sync[p++];
    if (p == 8)
      p = 0;
  }

  if (syncbuf.sync == SPIP_FRAME_SYNC) {
    self->pdu.sync = syncbuf.sync;
    self->p = 0;
    return TRUE;
  }

  return FALSE;
}

BOOL
spip_iface_write(spip_iface_t *iface, const struct spip_pdu *pdu)
{
  const uint8_t *as_bytes = (const uint8_t *) pdu;
  unsigned int size = pdu->size + SPIP_HEADER_SIZE;
  unsigned int i;

  if (iface->write != NULL) {
    return iface->write(iface->userdata, pdu, size);
  } else {
    if (iface->write_byte == NULL)
      return FALSE;

    for (i = 0; i < size; ++i)
      if (!(iface->write_byte)(iface->userdata, as_bytes[i]))
        return FALSE;
  }

  return TRUE;
}

BOOL
spip_iface_read(spip_iface_t *iface, struct spip_pdu **pdu)
{
  uint8_t c;
  uint32_t crc, computed_crc;
  spip_ctx_t *self = &iface->spip_ctx;
  union {
   uint8_t *pdu_data;
   struct spip_pdu *pdu_s;
  } updu;
  unsigned int i = 0;
  enum spip_loop_state state = SPIP_LOOP_STATE_SYNCING;

  if (iface->read_byte == NULL)
   return FALSE;

  updu.pdu_data = NULL;

  spip_ctx_resync(self);

  while ((iface->read_byte)(iface->userdata, &c)) {
    fflush(stdout);
    switch (state) {
      case SPIP_LOOP_STATE_SYNCING:
        if (spip_ctx_sync(self, c)) {
          state = SPIP_LOOP_READING_HEADER;
          i = 8;
        }

        break;

      case SPIP_LOOP_READING_HEADER:
        self->header[i++] = c;
        if (i == SPIP_HEADER_SIZE) {
          /* Got header. Validate CRC */
          crc = self->pdu.header_crc;
          self->pdu.header_crc = 0;

          computed_crc = spip_crc32b(
              NULL,
              self->header,
              SPIP_HEADER_SIZE);

          if (computed_crc != crc) {
            /* Bad CRC. Sync again */
            spip_ctx_resync(self);
#ifdef ERROR_REPORTING
#  ifdef __SAM4CMS16C_0__
            lcd_printf(1, "HDR!%08x", computed_crc);
#  else
            fprintf(
              stderr,
              "\033[0;31mBroken SPIP (UART) message (header CRC: %08x != %08x)\033[0m\r",
              crc,
              computed_crc);
#  endif /* __SAM4CMS16C_0__ */
#endif /* ERROR_REPORTING */
            state = SPIP_LOOP_STATE_SYNCING;
          } else {
            i = SPIP_HEADER_SIZE;
            /* Error checking not necessary here */
            updu.pdu_data = spip_ctx_assert_alloc(
                self,
                i + self->pdu.size);
            if (updu.pdu_data != NULL)
              memcpy(updu.pdu_data, self->header, SPIP_HEADER_SIZE);
            state = SPIP_LOOP_READING_BODY;
          }
        }
        break;

      case SPIP_LOOP_READING_BODY:
        if (updu.pdu_data != NULL)
          updu.pdu_data[i++] = c;

        if (i == (unsigned) self->pdu.size + SPIP_HEADER_SIZE) {
          if (updu.pdu_data != NULL) {
            crc = updu.pdu_s->crc;
            updu.pdu_s->crc = 0;

            computed_crc = spip_crc32b(NULL, updu.pdu_data, i);
            if (computed_crc == crc) {
              *pdu = updu.pdu_s;
              return TRUE;
            }
#ifdef ERROR_REPORTING
#  ifdef __SAM4CMS16C_0__
            lcd_printf(1, "BDY!%08x", computed_crc);
#  endif /* __SAM4CMS16C_0__ */
#endif /* ERROR_REPORTING */
          }

          spip_ctx_resync(self);
          state = SPIP_LOOP_STATE_SYNCING;
        }
        break;
    }
  }

  return FALSE;
}

void
spip_iface_dispose(spip_iface_t *iface, struct spip_pdu *pdu)
{
  /* NO-OP in this impl */

  (void) iface;
  (void) pdu;
}

struct spip_pdu *
spip_pdu_dup(const struct spip_pdu *pdu)
{
  struct spip_pdu *copy = malloc(SPIP_HEADER_SIZE + pdu->size);

  if (copy == NULL)
    return NULL;

  memcpy(copy, pdu, SPIP_HEADER_SIZE + pdu->size);

  return copy;
}

BOOL
spip_iface_close(spip_iface_t *iface)
{
  if (iface->close == NULL)
    return FALSE;

  return (iface->close) (iface->userdata);
}

/* Helper functions */
static BOOL
spip_iface_write_ex(
  spip_iface_t *iface,
  enum spip_command command,
  const void *data,
  size_t size)
{
  static struct spip_pdu *alloc = NULL;
  static size_t last_size = 0;
  void *tmp = NULL;
  BOOL ok = FALSE;

  TRY(size < 65536);

  if (alloc == NULL || size > last_size) {
    TRY(tmp = realloc(alloc, size + SPIP_HEADER_SIZE));
    alloc = tmp;

    if (last_size == 0) {
      alloc->sync = SPIP_FRAME_SYNC;
      alloc->pad = 0;
    }

    last_size = size;
  }

  alloc->command = command;
  alloc->size = size;
  alloc->crc = 0;
  alloc->header_crc = 0;

  memcpy(alloc->data, data, size);

  alloc->crc = spip_crc32b(
      NULL,
      (const uint8_t *) alloc,
      size + SPIP_HEADER_SIZE);

  alloc->header_crc = spip_crc32b(
      NULL,
      (const uint8_t *) alloc,
      SPIP_HEADER_SIZE);

  TRY(spip_iface_write(iface, alloc));

  ok = TRUE;

fail:
  return ok;
}

BOOL
spip_iface_write_frame(spip_iface_t *iface, const void *data, size_t size)
{
  return spip_iface_write_ex(iface, SPIP_COMMAND_FRAME, data, size);
}

BOOL
spip_iface_set_led_mask(spip_iface_t *iface, uint8_t mask)
{
  return spip_iface_write_ex(iface, SPIP_COMMAND_LED_SET_MASK, &mask, 1);
}

BOOL
spip_iface_toggle_led_mask(spip_iface_t *iface, uint8_t mask)
{
  return spip_iface_write_ex(iface, SPIP_COMMAND_LED_TOGGLE_MASK, &mask, 1);
}

BOOL
spip_iface_set_lcd(spip_iface_t *iface, uint8_t row, const char *text)
{
  char fullmsg[18];
  size_t len;

  memset(fullmsg, 0, sizeof(fullmsg));

  fullmsg[0] = row;

  len = strlen(text);

  if (len > 16)
    len = 16;

  memcpy(fullmsg + 1, text, len);

  return spip_iface_write_ex(iface, SPIP_COMMAND_LCD, fullmsg, sizeof(fullmsg));
}

BOOL
spip_iface_lcd_printf(
    spip_iface_t *iface,
    uint8_t row,
    const char *fmt,
    ...)
{
  BOOL ok = FALSE;
  char fullmsg[17];
  va_list ap;

  va_start(ap, fmt);

  vsnprintf(
    fullmsg,
    17,
    fmt,
    ap);

  fullmsg[16] = '\0';

  ok = spip_iface_set_lcd(iface, row, fullmsg);

  return ok;
}

