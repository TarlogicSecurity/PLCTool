/*
 * layer.h: PRIME 1.3 layer
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef _PRIME_LAYER_H
#define _PRIME_LAYER_H

#include "state.h"

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

prime13_layer_t *prime13_layer_new(
    const struct prime13_phy_adapter *adapter,
    const struct prime13_callbacks *callbacks);

BOOL prime13_layer_feed(prime13_layer_t *self, const void *data, size_t size);
BOOL prime13_layer_work(prime13_layer_t *self);

void prime13_layer_destroy(prime13_layer_t *self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRIME_LAYER_H */
