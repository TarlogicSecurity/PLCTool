/*
 * stream.h: Byte stream operations
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef _BER_STREAM_H
#define _BER_STREAM_H

#include <util/defs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct ber_stream {
  uint8_t *buffer;
  size_t size;
  size_t alloc;
  off_t pos;
  BOOL borrow;
  BOOL overflown;
};

typedef struct ber_stream ber_stream_t;

static inline void
ber_stream_mark_overflow(ber_stream_t *self)
{
  self->overflown = TRUE;
}

static inline BOOL
ber_stream_overflow(const ber_stream_t *self)
{
  return self->overflown;
}

ber_stream_t *ber_stream_new_ex(void *origin, size_t size, BOOL borrow);

ber_stream_t *ber_stream_copy(const void *origin, size_t size);
ber_stream_t *ber_stream_view(void *origin, size_t size);

BOOL ber_stream_read(ber_stream_t *self, void *buf, size_t size);
BOOL ber_stream_read_buffer(ber_stream_t *self, void *buf, size_t size);

#define ber_stream_read_uint8(self, buf)  ber_stream_read(self, &buf, 1)
#define ber_stream_read_uint16(self, buf) ber_stream_read(self, &buf, 2)
#define ber_stream_read_uint24(self, buf) ber_stream_read(self, &buf, 3)
#define ber_stream_read_uint32(self, buf) ber_stream_read(self, &buf, 4)
#define ber_stream_read_uint64(self, buf) ber_stream_read(self, &buf, 8)

size_t ber_stream_avail(const ber_stream_t *self);
BOOL ber_stream_seek(ber_stream_t *self, off_t pos);

void ber_stream_destroy(ber_stream_t *self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BER_STREAM_H */

