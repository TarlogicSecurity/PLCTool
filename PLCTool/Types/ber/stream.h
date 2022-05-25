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

#ifndef _BER_STREAM_H
#define _BER_STREAM_H

#include <PLCTool/util/defs.h>
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

#define ber_stream_read_uint8(self, buf) ber_stream_read(self, &buf, 1)
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
