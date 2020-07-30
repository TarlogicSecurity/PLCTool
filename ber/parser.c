/*
 * ber.c: A-XDR data parser
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#define logfunc(severity, fmt, arg...)

#include <ber/ber.h>
#include <string.h>
#include <ctype.h>

#define TRANSL2(x, y) \
  case JOIN(DLMS_DATA_TYPE_, x): \
    return JOIN(BER_CI_, y)

#define TRANSL(x) TRANSL2(x, x)

BER_CI
dlms_data_type_to_ci(DLMS_DATA_TYPE type)
{
  switch (type) {
    TRANSL(BOOLEAN);
    TRANSL(BIT_STRING);
    TRANSL(INT32);
    TRANSL(UINT32);
    TRANSL(OCTET_STRING);
    TRANSL(STRING);
    TRANSL(BINARY_CODED_DESIMAL);
    TRANSL(STRING_UTF8);
    TRANSL(INT8);
    TRANSL(INT16);
    TRANSL(UINT8);
    TRANSL(UINT16);
    TRANSL(INT64);
    TRANSL(UINT64);
    TRANSL(ENUM);
    TRANSL(FLOAT32);
    TRANSL(FLOAT64);

    default:
      return BER_CI_NULL;
  }
}

static BOOL
dlms_data_type_is_vectorial(DLMS_DATA_TYPE type)
{
  return type == DLMS_DATA_TYPE_STRUCTURE ||
      type == DLMS_DATA_TYPE_ARRAY ||
      type == DLMS_DATA_TYPE_BIT_STRING ||
      type == DLMS_DATA_TYPE_OCTET_STRING ||
      type == DLMS_DATA_TYPE_STRING ||
      type == DLMS_DATA_TYPE_BINARY_CODED_DESIMAL ||
      type == DLMS_DATA_TYPE_STRING_UTF8;
}

BOOL
ber_stream_read_objcount(ber_stream_t *self, size_t *len)
{
  uint8_t lenbyte;
  uint8_t lenlen;

  BOOL ok = FALSE;

  TRY(ber_stream_read_uint8(self, lenbyte));

  if (lenbyte <= 0x80) {
    *len = lenbyte;
  } else {
    lenlen = lenbyte & 0x7f;
    TRY(lenlen < sizeof(size_t));

    *len = 0;

    /* FIXME: THIS IS NOT ENOUGH FOR BIG ENDIAN SYSTEMS */
    TRY(ber_stream_read(self, len, lenlen));
  }

  ok = TRUE;

fail:
  return ok;
}

ber_type_t *
ber_type_from_dlms_data(ber_stream_t *stream)
{
  ber_type_t *new = NULL;
  ber_type_t *result = NULL;
  ber_type_t *entry = NULL;
  uint8_t type;
  size_t size = 0;
  unsigned int i;

  TRY(ber_stream_read_uint8(stream, type));
  TRY(new = ber_type_new_from_dlms(type));

  if (dlms_data_type_is_vectorial(type))
    TRY(ber_stream_read_objcount(stream, &size));

  switch (type) {
    case DLMS_DATA_TYPE_INT8:
    case DLMS_DATA_TYPE_UINT8:
      TRY(ber_stream_read_uint8(stream, new->as_byte));
      break;

    case DLMS_DATA_TYPE_INT16:
    case DLMS_DATA_TYPE_UINT16:
      TRY(ber_stream_read_uint16(stream, new->as_int));
      break;

    case DLMS_DATA_TYPE_INT32:
    case DLMS_DATA_TYPE_UINT32:
      TRY(ber_stream_read_uint32(stream, new->as_long));
      break;


    case DLMS_DATA_TYPE_INT64:
    case DLMS_DATA_TYPE_UINT64:
      TRY(ber_stream_read_uint64(stream, new->as_quad));
      break;

    case DLMS_DATA_TYPE_BIT_STRING:
      size = (size >> 3) + !!(size & 7);
      break;

    case DLMS_DATA_TYPE_OCTET_STRING:
    case DLMS_DATA_TYPE_STRING:
    case DLMS_DATA_TYPE_STRING_UTF8:
      TRY(ber_type_set_buffer(new, NULL, size));
      TRY(ber_stream_read_buffer(stream, new->bytebuf, size));
      break;

    case DLMS_DATA_TYPE_ARRAY:
    case DLMS_DATA_TYPE_STRUCTURE:
      for (i = 0; i < size; ++i) {
        TRY(entry = ber_type_from_dlms_data(stream));
        TRY(ber_type_push(new, entry));
        entry = NULL;
      }
      break;

    default:
      log_error("Unsupported DLMS type 0x%x\n", type);
      goto fail;
  }

  result = new;
  new = NULL;

fail:
  if (new != NULL)
    ber_type_destroy(new);

  if (entry != NULL)
    ber_type_destroy(entry);

  return result;
}

ber_type_t *
ber_type_from_stream(ber_stream_t *stream)
{
  ber_type_t *result = NULL;
  ber_type_t *new = NULL;
  ber_type_t *entry = NULL;
  ber_stream_t *borrow = NULL;
  uint8_t id;
  size_t size;
  size_t ideal = 4;
  off_t pos = 0;

  TRY(ber_stream_read_uint8(stream, id));
  TRY(new = ber_type_new(id));
  TRY(ber_stream_read_objcount(stream, &size));
  if (size > ber_stream_avail(stream)) {
    ber_stream_mark_overflow(stream);
    goto fail;
  }
  pos = stream->pos + size;

  if (new->constructed) {
    /* Constructed type */
    TRY(borrow = ber_stream_view(stream->buffer + stream->pos, size));

    /* Process in loop */
    while (ber_stream_avail(borrow) > 0) {
      TRY(entry = ber_type_from_stream(borrow));
      TRY(ber_type_push(new, entry));
      entry = NULL;
    }
  } else if (ber_type_is_buffer(new)) {
    /* Just read all these bytes */
    TRY(ber_type_set_buffer(new, NULL, size));
    TRY(ber_stream_read_buffer(stream, new->bytebuf, size));
  } else {
    /* Atomic types */
    switch (new->ci) {
      case BER_CI_EOC:
        break;

      case BER_CI_BOOLEAN:
        ideal = sizeof(uint8_t);
        break;

      case BER_CI_INTEGER:
        ideal = sizeof(uint16_t);
        break;

      case BER_CI_REAL:
        ideal = sizeof(uint32_t);
        break;

      default:
        log_error("Unsupported BER type 0x%x -> 0x%x (len %d)\n", id, new->ci, size);
        ber_hexdump(stream->buffer, stream->size);
        goto fail;
    }

    if (size > ideal) {
        log_error("Ideal type for 0x%x is %d, but its length is %d??\n", new->ci, ideal, size);
        goto fail;
    }

    TRY(ber_stream_read(stream, new->scalbuf, size));

  }

  result = new;
  new = NULL;

fail:
  if (pos != 0)
    stream->pos = pos;

  if (borrow != NULL)
    ber_stream_destroy(borrow);

  if (new != NULL)
    ber_type_destroy(new);

  if (entry != NULL)
    ber_type_destroy(entry);

  return result;
}

static void
ber_type_debug_internal(
    const ber_type_t *self,
    FILE *fp,
    unsigned int level)
{
  int i;

  for (i = 0; i < (int) level; ++i)
    printf("  ");

  if (self->private_type)
    printf("private ");

  if (self->application)
    printf("application ");

  if (self->context)
    printf("context ");

  if (self->constructed) {
    if (self->array)
      printf("array[%d] = {\n", self->field_count);
    else
      printf("struct stru_%d {\n", self->id);
    for (i = 0; i < self->field_count; ++i)
      ber_type_debug_internal(self->field_list[i], fp, level + 1);

    for (i = 0; i < (int) level; ++i)
      printf("  ");
    printf("};\n");
  } else if (ber_type_is_buffer(self)) {
      switch (self->ci) {
        case BER_CI_OCTET_STRING:
          printf("uint8_t  octets[%lu] = {", self->bytecount);
          break;

        case BER_CI_EOC:
          printf("uint8_t  internal[%lu] = {", self->bytecount);
          break;

        case BER_CI_OBJECT_IDENTIFIER:
          printf("uint8_t  obj_ident[%lu] = {", self->bytecount);
          break;

        case BER_CI_UTC_TIME:
          printf("time_t  utctime = ");
          break;

        default:
          printf("uint8_t  generic_buffer_%d[%lu] = {", self->ci, self->bytecount);
      }

    for (i = 0; i < (int) self->bytecount; ++i) {
      printf("%s0x%02x", i > 0 ? ", " : "", self->bytebuf[i]);
    }
    printf("};\n");
  } else {
    switch (self->ci) {
      case BER_CI_UINT8:
        printf("uint8_t byte = %d;\n", self->as_byte);
        break;

      case BER_CI_INT8:
        printf("int8_t sbyte = %d;\n", self->as_sbyte);
        break;

      case BER_CI_UINT16:
        printf("uint16_t int = %d;\n", self->as_int);
        break;

      case BER_CI_INT16:
        printf("int16_t sint = %d;\n", self->as_sint);
        break;

      case BER_CI_UINT32:
        printf("uint32_t long = %u;\n", self->as_long);
        break;

      case BER_CI_INT32:
        printf("int32_t slong = %d;\n", self->as_slong);
        break;

      case BER_CI_UINT64:
        printf("uint64_t quad = %ld;\n", self->as_quad);
        break;

      case BER_CI_INT64:
        printf("int64_t squad = %lu;\n", self->as_squad);
        break;

      case BER_CI_BOOLEAN:
        printf("bool     logicval = %s;\n", self->as_boolean ? "true" : "false");
        break;

      default:
        printf("unknown_scalar_%d;\n", self->ci);
    }
  }
}

void
ber_type_debug(const ber_type_t *self, FILE *fp)
{
  ber_type_debug_internal(self, fp, 1);
}

