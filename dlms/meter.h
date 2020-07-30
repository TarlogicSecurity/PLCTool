/*
 * meter.h: Meter context
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef _DLMS_METER_H
#define _DLMS_METER_H

#include <util/util.h>
#include <PRIME/libprime/prime.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PRIME_BUFFER_SIZE 256u
#define PRIME_PDU_SIZE 1024u

struct meter_block {
  union {
    void *buffer;
    const uint8_t *as_bytes;
  };
  size_t size;
};

typedef struct meter_block meter_block_t;

struct meter_rq_buffer {
  uint8_t invokeId;
  BOOL complete;
  PTR_LIST(meter_block_t, block);
};

typedef struct meter_rq_buffer meter_rq_buffer_t;

struct meter_context {
  prime13_node_t *node;
  prime13_layer_t *layer;
  BOOL have_model;
  char model[3];
  uint8_t serial[5];
  uint8_t frame[PRIME_BUFFER_SIZE + 9];
  uint8_t pdu[PRIME_PDU_SIZE];

  PTR_LIST(meter_rq_buffer_t, request);
};

typedef struct meter_context meter_context_t;

void meter_block_destroy(meter_block_t *self);
meter_block_t *meter_block_new(const void *data, size_t size);
void meter_rq_buffer_destroy(meter_rq_buffer_t *self);
BOOL meter_rq_buffer_set_block(
    meter_rq_buffer_t *self,
    const void *data,
    size_t size,
    unsigned int index,
    BOOL last);
BOOL meter_rq_buffer_coalesce(
    const meter_rq_buffer_t *buffer,
    uint8_t **result,
    size_t *size);
meter_rq_buffer_t *meter_rq_buffer_new(uint8_t invoke);
meter_rq_buffer_t *meter_context_lookup_request(meter_context_t *self, uint8_t invoke);
meter_rq_buffer_t *meter_context_assert_request(meter_context_t *self, uint8_t invoke);
void meter_context_remove_request(meter_context_t *self, uint8_t invoke);
void meter_context_clear_requests(meter_context_t *self);
void meter_context_destroy(meter_context_t *self);
prime13_layer_t *meter_context_get_layer(const meter_context_t *self);
meter_context_t *meter_context_new(prime13_layer_t *layer, prime13_node_t *node);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DLMS_METER_H */
