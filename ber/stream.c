/*
 * stream.c: Byte stream operations
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#include <ber/stream.h>
#include <string.h>

ber_stream_t *
ber_stream_new_ex(void *origin, size_t size, BOOL borrow)
{
  ber_stream_t *self = NULL;

  ALLOCATE(self, ber_stream_t);

  self->borrow = borrow;
  self->alloc = self->size = size;

  if (borrow) {
    self->buffer = origin;
  } else {
    ALLOCATE_MANY(self->buffer, size, uint8_t);
    memcpy(self->buffer, origin, size);
  }

  return self;

fail:
  if (self != NULL)
    ber_stream_destroy(self);

  return NULL;
}

ber_stream_t *
ber_stream_copy(const void *origin, size_t size)
{
  return ber_stream_new_ex((void *) origin, size, FALSE);
}

ber_stream_t *
ber_stream_view(void *origin, size_t size)
{
  return ber_stream_new_ex(origin, size, TRUE);
}

static BOOL
ber_stream_read_ex(ber_stream_t *self, void *buf, size_t size, BOOL rev)
{
  size_t avail = self->size - self->pos;
  uint8_t *as_bytes = (uint8_t *) buf;
  uint8_t tmp;
  unsigned int i;

  if (avail < size) {
    ber_stream_mark_overflow(self);
    return FALSE;
  }

  memcpy(as_bytes, self->buffer + self->pos, size);
  self->pos += size;

  if (rev) {
    for (i = 0; i < (size >> 1); ++i) {
      tmp = as_bytes[i];
      as_bytes[i] = as_bytes[size - i - 1];
      as_bytes[size - i - 1] = tmp;
    }
  }

  return TRUE;
}

BOOL
ber_stream_read_buffer(ber_stream_t *self, void *buf, size_t size)
{
  return ber_stream_read_ex(self, buf, size, FALSE);
}

BOOL
ber_stream_read(ber_stream_t *self, void *buf, size_t size)
{
/* Only in little endian */
  return ber_stream_read_ex(self, buf, size, TRUE);
}

size_t
ber_stream_avail(const ber_stream_t *self)
{
  return self->size - self->pos;
}

BOOL
ber_stream_seek(ber_stream_t *self, off_t pos)
{
  if (pos < 0 || pos > (off_t) self->size)
    return FALSE;

  self->pos = pos;

  return TRUE;
}

void
ber_stream_destroy(ber_stream_t *self)
{
  if (!self->borrow && self->buffer != NULL)
    free(self->buffer);

  free(self);
}
