/*
 * layer.c: PRIME 1.3 layer
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#include "layer.h"

prime13_layer_t *
prime13_layer_new(
    const struct prime13_phy_adapter *adapter,
    const struct prime13_callbacks *callbacks)
{
  prime13_layer_t *new = NULL;

  ALLOCATE(new, prime13_layer_t);

  CONSTRUCT(new->state, prime13_state);

  new->adapter   = *adapter;
  new->callbacks = *callbacks;

  return new;

fail:
  if (new != NULL)
    prime13_layer_destroy(new);

  return NULL;
}

BOOL
prime13_layer_feed_extended(
    prime13_layer_t *self,
    const struct timeval *timestamp,
    const void *data,
    size_t size)
{
  const struct prime13_state_pdu_info *info;
  prime13_beacon_record_t *beacon_record;
  const uint8_t *pdata;
  void *userdata;
  size_t psize;
  BOOL ok = FALSE;

  TRY(prime13_state_feed(self->state, data, size));

  if (timestamp != NULL)
    self->feed_time = *timestamp;

  info = prime13_state_get_pdu_info(self->state);
  beacon_record = prime13_state_get_beacon_record(self->state);

  /* Generic Prime frame callback */
  if (self->callbacks.on_frame != NULL) {
    if (info != NULL) {
      if (!(self->callbacks.on_frame) (
          self->callbacks.userdata,
          info->node->subnet->sna,
          info->direction,
          data,
          size))
        goto fail;
    } else if (beacon_record != NULL) {
      if (!(self->callbacks.on_frame) (
          self->callbacks.userdata,
          beacon_record->sna,
          FALSE,
          data,
          size))
        goto fail;
    }
  }

  /* Beacon callback */
  if (self->callbacks.on_subnet != NULL) {
    if (beacon_record != NULL)
      if (!(self->callbacks.on_subnet)(
          self,
          beacon_record->sna,
          beacon_record->count,
          self->callbacks.userdata))
        goto fail;
  }

  /* New meter found callback */
  if (self->callbacks.on_meter != NULL && info != NULL) {
    if (prime13_node_is_new(info->node)) {
      prime13_node_ack(info->node);
      if ((userdata = (self->callbacks.on_meter) (self, info->node)) == NULL)
        goto fail;

      info->node->userdata = userdata;
    }
  }

  /* Data callback */
  if (self->callbacks.on_data != NULL) {
    if (info != NULL) {
      if (info->data) {
        /* Data packets require assembling */
        if (prime13_node_assemble(
          info->node,
          info->direction,
          &pdata,
          &psize)) {
          if (!(self->callbacks.on_data) (
            self,
            info->node,
            info->node->userdata,
            info->direction,
            pdata,
            psize))
            goto fail;
        }
      }        
    }
  }

  if (info != NULL) {
    prime13_node_set_data_ephemereal_context(info->node, TRUE,  NULL);
    prime13_node_set_data_ephemereal_context(info->node, FALSE, NULL);
  }
  
  ok = TRUE;
  
fail:

  return ok;
}

BOOL
prime13_layer_feed(
    prime13_layer_t *self,
    const void *data,
    size_t size)
{
  struct timeval tv;

  gettimeofday(&tv, NULL);

  return prime13_layer_feed_extended(self, &tv, data, size);
}

BOOL
prime13_layer_work(prime13_layer_t *self)
{
  const void *data = NULL;
  size_t size = 0;
  
  BOOL ok = FALSE;

  if (self->disconnected)
  goto fail;

  TRY((self->adapter.read) (self->adapter.userdata, &data, &size));

  if (data != NULL) {
    TRY(prime13_layer_feed(self, data, size));
  } else {
    self->disconnected = TRUE;
  }

  ok = TRUE;

  fail:
  if (data != NULL)
    if (self->adapter.dispose != NULL)
      (self->adapter.dispose) (self->adapter.userdata, data, size);

  return ok;
}

void *
prime13_state_for_each_node(
    prime13_state_t *self,
    void *(*func) (
        prime13_state_t *self,
        prime13_node_t *node,
        void *context),
    void *context)
{
  int i;
  void *ret = NULL;

  for (i = 0; i < self->node_count; ++i)
    if (self->node_list[i] != NULL)
      if ((ret = (func) (self, self->node_list[i], context)) != NULL)
        break;

  return ret;
}

static void *
prime13_layer_trigger_node_dispose(
    prime13_state_t *self,
    prime13_node_t *node,
    void *ctx)
{
  prime13_layer_t *layer = (prime13_layer_t *) ctx;

  (void) self; // stfu

  if (node->userdata != NULL)
    (layer->callbacks.dispose)(node->userdata);

  return NULL;
}

void
prime13_layer_destroy(prime13_layer_t *self)
{
  if (self->callbacks.dispose != NULL)
    (void) prime13_state_for_each_node(
        self->state,
        prime13_layer_trigger_node_dispose,
        self);

  if (self->callbacks.destroy != NULL)
    (void) self->callbacks.destroy(self->callbacks.userdata);

  if (self->state != NULL)
    prime13_state_destroy(self->state);

  free(self);
}

