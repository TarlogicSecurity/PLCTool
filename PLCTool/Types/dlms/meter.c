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

#include "meter.h"

#include <PLCTool/util/defs.h>
#include <string.h>

void
meter_block_destroy(meter_block_t *self)
{
  if (self->buffer != NULL)
    free(self->buffer);

  free(self);
}

meter_block_t *
meter_block_new(const void *data, size_t size)
{
  meter_block_t *new = NULL;
  meter_block_t *result = NULL;

  ALLOCATE(new, meter_block_t);
  TRY(new->buffer = malloc(size));

  memcpy(new->buffer, data, size);
  new->size = size;
  
  return new;

fail:
  if (new != NULL)
    meter_block_destroy(new);

  return result;
}

void
meter_rq_buffer_destroy(meter_rq_buffer_t *self)
{
  int i;

  for (i = 0; i < self->block_count; ++i)
    if (self->block_list[i] != NULL)
      meter_block_destroy(self->block_list[i]);

  if (self != NULL)
    free(self);
}

BOOL
meter_rq_buffer_set_block(
    meter_rq_buffer_t *self,
    const void *data,
    size_t size,
    unsigned int index,
    BOOL last)
{
  unsigned int count;
  unsigned int new_size;
  meter_block_t *block = NULL;
  meter_block_t **temp;
  BOOL ok = FALSE;

  CONSTRUCT(block, meter_block, data, size);

  if (index >= (unsigned) self->block_count) {
    if (self->complete) {
      ok = TRUE;
      goto fail;
    }

    new_size = index + 1;
    count = new_size - self->block_count;

    TRY(temp = realloc(
        self->block_list,
        new_size * sizeof (meter_block_t *)));

    memset(temp + self->block_count, 0, sizeof (meter_block_t *) * count);

    self->block_list = temp;
    self->block_count = new_size;

    if (last)
      self->complete = TRUE;

  } else {
    if (self->block_list[index] != NULL)
      meter_block_destroy(self->block_list[index]);
  }

  self->block_list[index] = block;
  block = NULL;

  ok = TRUE;

fail:
  if (block != NULL)
    meter_block_destroy(block);

  return ok;
}

BOOL
meter_rq_buffer_coalesce(
    const meter_rq_buffer_t *buffer,
    uint8_t **result,
    size_t *size)
{
  BOOL ok = FALSE;
  size_t total_size = 0, p = 0;
  int i;

  TRY(buffer->complete);

  for (i = 0; i < buffer->block_count; ++i) {
    if (buffer->block_list[i] == NULL)
      goto fail;

    total_size += buffer->block_list[i]->size;
  }

  TRY(*result = malloc(total_size));
  *size = total_size;

  for (i = 0; i < buffer->block_count; ++i) {
    memcpy(
        *result + p,
        buffer->block_list[i]->buffer,
        buffer->block_list[i]->size);
    p += buffer->block_list[i]->size;
  }

  ok = TRUE;

fail:
  return ok;
}

meter_rq_buffer_t *
meter_rq_buffer_new(uint8_t invoke)
{
  meter_rq_buffer_t *new = NULL;

  ALLOCATE(new, meter_rq_buffer_t);

  new->invokeId = invoke;

fail:
  return new;
}

meter_rq_buffer_t *
meter_context_lookup_request(meter_context_t *self, uint8_t invoke)
{
  int i;

  for (i = 0; i < self->request_count; ++i)
    if (self->request_list[i] != NULL)
      if (self->request_list[i]->invokeId == invoke)
        return self->request_list[i];

  return NULL;
}

meter_rq_buffer_t *
meter_context_assert_request(meter_context_t *self, uint8_t invoke)
{
  meter_rq_buffer_t *new = NULL;
  meter_rq_buffer_t *result = NULL;

  if ((result = meter_context_lookup_request(self, invoke)) == NULL) {
    TRY(new = meter_rq_buffer_new(invoke));
    PTR_LIST_APPEND_CHECK(self->request, new);
    result = new;
    new = NULL;
  }

fail:
  if (new != NULL)
    meter_rq_buffer_destroy(new);

  return result;
}

void
meter_context_remove_request(meter_context_t *self, uint8_t invoke)
{
  int i;

  for (i = 0; i < self->request_count; ++i)
    if (self->request_list[i] != NULL)
      if (self->request_list[i]->invokeId == invoke) {
        meter_rq_buffer_destroy(self->request_list[i]);
        self->request_list[i] = NULL;
      }
}

void
meter_context_clear_requests(meter_context_t *self)
{
  int i;

  for (i = 0; i < self->request_count; ++i)
    if (self->request_list[i] !=  NULL)
      meter_rq_buffer_destroy(self->request_list[i]);

  if (self->request_list != NULL)
    free(self->request_list);

  self->request_list = NULL;
  self->request_count = 0;
}

void
meter_context_destroy(meter_context_t *self)
{
  meter_context_clear_requests(self);
  free(self);
}


prime13_layer_t *
meter_context_get_layer(const meter_context_t *self)
{
  return self->layer;
}

meter_context_t *
meter_context_new(prime13_layer_t *layer, prime13_node_t *node)
{
  meter_context_t *new = NULL;

  ALLOCATE(new, meter_context_t);
  new->node = node;
  new->layer = layer;
  new->have_model = FALSE;
  
  return new;

fail:
  if (new != NULL)
    meter_context_destroy(new);

  return NULL;
}

