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

#ifndef _PRIME_LAYER_H
#define _PRIME_LAYER_H

#include "state.h"
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct prime13_phy_adapter {
  void *userdata;
  BOOL (*read) (void *userdata, const void **data, size_t *size);
  BOOL (*write) (void *userdata, const void *data, size_t size);
  void (*dispose) (void *userdata, const void *data, size_t size);
};

#define prime13_phy_adapter_INITIALIZER \
  {   \
    NULL, /* userdata */ \
    NULL, /* read */ \
    NULL, /* write */\
    NULL, /* dispose */ \
}

struct prime13_layer;

struct prime13_callbacks {
  void *userdata;

  void *(*on_meter) (
      struct prime13_layer *self,
      prime13_node_t *node);

  BOOL (*on_subnet) (
      struct prime13_layer *self,
      const uint8_t *sna,
      uint64_t times,
      void *userdata);

  BOOL (*on_frame) (
      void *userdata,
      const uint8_t *sna,
      BOOL direction,
      const void *data,
      size_t size);

  BOOL (*on_data) (
      struct prime13_layer *self,
      prime13_node_t *node,
      void *userdata,
      BOOL downstream,
      const void *data,
      size_t size);

  void (*dispose) (void *userdata);

  void (*destroy) (void *cbdata);
};

#define prime13_callbacks_INITIALIZER \
{ \
    NULL, /* userdata */ \
    NULL, /* on_meter */ \
    NULL, /* on_subnet */ \
    NULL, /* on_frame */ \
    NULL, /* on_data */ \
    NULL, /* dispose */ \
    NULL, /* destroy */ \
}

struct prime13_layer {
  prime13_state_t *state;
  BOOL disconnected;
  struct timeval feed_time;
  struct prime13_phy_adapter adapter;
  struct prime13_callbacks callbacks;
};

typedef struct prime13_layer prime13_layer_t;

static inline BOOL
prime13_layer_closed(const prime13_layer_t *self)
{
  return self->disconnected;
}

static inline void *
prime13_layer_get_userdata(const prime13_layer_t *self)
{
  return self->callbacks.userdata;
}

static inline struct timeval
prime13_layer_get_feed_time(const prime13_layer_t *self)
{
  return self->feed_time;
}

prime13_layer_t *prime13_layer_new(
    const struct prime13_phy_adapter *adapter,
    const struct prime13_callbacks *callbacks);

BOOL prime13_layer_feed_extended(
    prime13_layer_t *self,
    const struct timeval *timestamp,
    const void *data,
    size_t size);
BOOL prime13_layer_feed(prime13_layer_t *self, const void *data, size_t size);
BOOL prime13_layer_work(prime13_layer_t *self);

void prime13_layer_destroy(prime13_layer_t *self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRIME_LAYER_H */
