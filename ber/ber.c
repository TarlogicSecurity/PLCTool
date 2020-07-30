/*
 * ber.c: A-XDR BER types
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#include <ber/ber.h>
#include <string.h>
#include <ctype.h>

void
ber_fhexdump(FILE *fp, const void *data, uint32_t size)
{
  const uint8_t *bytes = (const uint8_t *) data;

  unsigned int i, j;

  for (i = 0; i < size; ++i) {
    if ((i & 0xf) == 0)
      fprintf(fp, "//  %08x  ", i);

    fprintf(fp, "%s%02x ", (i & 0xf) == 8 ? " " : "", bytes[i]);

    if ((i & 0xf) == 0xf) {
      fprintf(fp, " | ");

      for (j = i - 15; j <= i; ++j)
        fputc(isprint (bytes[j]) ? bytes[j] : '.', fp);

      fputc('\n', fp);
    }
  }

  if ((i & 0xf) != 0) {
    for (j = i; j < __ALIGN (size, 16); ++j)
      fprintf(fp, "   %s", (j & 0xf) == 8 ? " " : "");

    fprintf(fp, " | ");

    for (j = i & ~0xf; j < size; ++j)
      fputc(isprint (bytes[j]) ? bytes[j] : '.', fp);

    fputc('\n', fp);
  }

  fprintf(fp, "//  %08x  \n\n", i);
}

void
ber_hexdump(const void *data, uint32_t size)
{
  ber_fhexdump(stderr, data, size);
}

BOOL
ber_type_is_buffer(const ber_type_t *self)
{
  if (self->constructed)
    return FALSE;

  return self->private_type ||
      self->ci == BER_CI_EOC ||
      self->ci == BER_CI_BIT_STRING ||
      self->ci == BER_CI_OBJECT_IDENTIFIER ||
      self->ci == BER_CI_OCTET_STRING ||
      self->ci == BER_CI_UTF8_STRING ||
      self->ci == BER_CI_NUMERIC_STRING ||
      self->ci == BER_CI_PRINTABLE_STRING ||
      self->ci == BER_CI_TELETEX_STRING ||
      self->ci == BER_CI_VIDEOTEX_STRING ||
      self->ci == BER_CI_IA5_STRING ||
      self->ci == BER_CI_GRAPHIC_STRING ||
      self->ci == BER_CI_VISIBLE_STRING ||
      self->ci == BER_CI_GENERAL_STRING ||
      self->ci == BER_CI_UNIVERSAL_STRING ||
      self->ci == BER_CI_UTC_TIME ||
      self->ci == BER_CI_BMP_STRING;
}

void
ber_type_destroy(ber_type_t *self)
{
  int i;

  if (ber_type_is_buffer(self)) {
    if (self->bytebuf != NULL)
      free(self->bytebuf);
  } else if (self->constructed) {
    for (i = 0; i < self->field_count; ++i)
      if (self->field_list[i] != NULL)
        ber_type_destroy(self->field_list[i]);

    if (self->field_list != NULL)
      free(self->field_list);
  }

  free(self);
}

ber_type_t *
ber_type_new_from_dlms(DLMS_DATA_TYPE type)
{
  ber_type_t *self = NULL;

  ALLOCATE(self, ber_type_t);

  self->orig_id = type;

  switch (type) {
    case DLMS_DATA_TYPE_ARRAY:
      self->array = TRUE;
      /* This is intentional */

    case DLMS_DATA_TYPE_STRUCTURE:
      self->constructed = TRUE;
      break;

    default:
      if ((self->ci = dlms_data_type_to_ci(type)) == BER_CI_NULL) {
        log_error("Unmappeable type 0x%x\n", type);
        goto fail;
      }
  }

  return self;

fail:
  if (self != NULL)
    ber_type_destroy(self);

  return NULL;
}


ber_type_t *
ber_type_new(uint8_t id)
{
  uint8_t ci = id & (~(BER_CI_PRIVATE | BER_CI_CONTEXT | BER_CI_APPLICATION | BER_CI_CONSTRUCTED));
  uint8_t private = !!(id & BER_CI_PRIVATE);
  uint8_t context = !!(id & BER_CI_CONTEXT);
  uint8_t application = !!(id & BER_CI_APPLICATION);
  uint8_t constructed = !!(id & BER_CI_CONSTRUCTED);
  ber_type_t *self = NULL;

  ALLOCATE(self, ber_type_t);

  self->orig_id = id;
  self->private_type = private;
  self->ci = ci;
  self->context = context;
  self->application = application;
  self->constructed = constructed;

  return self;

fail:
  if (self != NULL)
    ber_type_destroy(self);

  return NULL;
}

BOOL
ber_type_set_buffer(ber_type_t *self, const void *buf, size_t size)
{
  BOOL ok = FALSE;
  void *dup = NULL;

  TRY(ber_type_is_buffer(self));

  ALLOCATE_MANY(dup, size, uint8_t);

  if (buf != NULL)
    memcpy(dup, buf, size);

  if (self->bytebuf != NULL)
    free(self->bytebuf);

  self->bytebuf = dup;
  self->bytecount = size;
  dup = NULL;

  ok = TRUE;

fail:
  if (dup != NULL)
    free(dup);

  return ok;
}

BOOL
ber_type_push(ber_type_t *self, ber_type_t *entry)
{
  BOOL ok = FALSE;

  TRY(self->constructed);

  TRYC(PTR_LIST_APPEND_CHECK(self->field, entry));

  ok = TRUE;

fail:
  return ok;
}

