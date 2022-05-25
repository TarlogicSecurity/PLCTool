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

#define logfunc(severity, fmt, arg...)

#include "dlms.h"

#include <stdio.h>
#include <string.h>

BOOL
dlms_process_get_request(
    meter_context_t *self,
    ber_stream_t *stream,
    dlms_message_t **omsg)
{
  dlms_message_t *message = NULL;
  ber_type_t *type = NULL;
  uint8_t cmd;
  uint8_t kind;
  uint8_t invokeId;
  uint8_t status;
  uint8_t len;
  uint8_t obis[7];
  BOOL ok = FALSE;
  
  *omsg = NULL;
  
  TRY(ber_stream_read_uint8(stream, cmd));
  TRY(ber_stream_read_uint8(stream, kind));
  TRY(ber_stream_read_uint8(stream, invokeId));

  switch (kind) {
    case DLMS_GET_COMMAND_TYPE_NORMAL:
      TRY(ber_stream_read_uint8(stream, status));
      TRY(ber_stream_read_uint8(stream, len));

      CONSTRUCT(message, dlms_message, cmd, invokeId);
      message->get_request.status = status;
      
      if (status == 0) {
        if (len != 7) {
          log_warning("Unsupported OBIS len %d\n", len);
          goto fail;
        }
      }

      TRY(ber_stream_read_buffer(stream, obis, 7));
      
      memcpy(message->get_request.obis, obis, 6);
      message->get_request.access = obis[6];

      TRY(type = ber_type_from_dlms_data(stream));
      
      /* Transfer type */
      TRY(dlms_message_give_type(message, type));
      type = NULL;
      
      (void) meter_context_remove_request(self, invokeId);
      
      /* Transfer message */
      *omsg = message;
      message = NULL;
      
      break;

    case DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK:
      /* Ask for next block. We can safely ignore this one */
      break;

    default:
      log_warning("Unsupported DLMS GET_REQUEST kind %d\n", kind);
      /* Nothing */
  }

  ok = TRUE;

fail:
  if (type != NULL)
    ber_type_destroy(type);

  if (message != NULL)
    dlms_message_destroy(message);

  return ok;
}

BOOL
dlms_process_get_response(
    meter_context_t *self,
    ber_stream_t *stream,
    dlms_message_t **omsg)
{
  BOOL ok = FALSE;
  dlms_message_t *message = NULL;
  ber_stream_t *view = NULL;
  ber_type_t *type = NULL;
  meter_rq_buffer_t *rq = NULL;
  uint8_t *buffer = NULL;
  size_t size;
  uint8_t cmd;
  uint8_t kind;
  uint8_t last, byte;
  uint8_t invokeId;
  uint32_t block;

  TRY(ber_stream_read_uint8(stream, cmd));
  TRY(ber_stream_read_uint8(stream, kind));
  TRY(ber_stream_read_uint8(stream, invokeId));

  *omsg = NULL;
  
  switch (kind) {
    case DLMS_GET_COMMAND_TYPE_NORMAL:
      CONSTRUCT(message, dlms_message, cmd, invokeId);
      
      TRY(ber_stream_read_uint8(stream, byte)); /* Status */
      message->get_response.kind   = kind;
      message->get_response.status = byte;
      
      TRY(type = ber_type_from_dlms_data(stream));
      TRY(dlms_message_give_type(message, type));
      type = NULL;
        
      /* Transfer message */
      *omsg = message;
      message = NULL;
      
      break;

    case DLMS_GET_COMMAND_TYPE_NEXT_DATA_BLOCK:
      TRY(rq = meter_context_assert_request(self, invokeId));

      TRY(ber_stream_read_uint8(stream, last));
      TRY(ber_stream_read_uint32(stream, block));
      TRY(ber_stream_read_uint8(stream, byte));

      if (block == 1)
        meter_context_remove_request(self, invokeId);
        
      /* Successful retrieval */
      if (byte == 0) {
        TRY(ber_stream_read_objcount(stream, &size));
        TRY(buffer = malloc(size));
        TRY(ber_stream_read_buffer(stream, buffer, size));
        TRY(meter_rq_buffer_set_block(rq, buffer, size, block - 1, last));
        free(buffer);
        buffer = NULL;

        if (rq->complete) {
          if (meter_rq_buffer_coalesce(rq, &buffer, &size)) {
            /* Coalesce successful. We have a message! */
            CONSTRUCT(message, dlms_message, cmd, invokeId);
            message->get_response.kind   = kind;
            message->get_response.status = byte;
            
            TRY(view = ber_stream_view(buffer, size));
            
            TRY(type = ber_type_from_dlms_data(view));
            TRY(dlms_message_give_type(message, type));
            type = NULL;
            
            meter_context_remove_request(self, invokeId);
            
            /* Transfer message */
            *omsg = message;
            message = NULL;
          }
        }
      } else {
        meter_context_remove_request(self, invokeId);
      }
      break;

    default:
      log_warning("Unsupported DLMS GET_RESPONSE kind %d\n", kind);
  }

  ok = TRUE;

fail:
  if (message != NULL)
    dlms_message_destroy(message);
    
  if (view != NULL)
    ber_stream_destroy(view);

  if (type != NULL)
    ber_type_destroy(type);
    
  if (buffer != NULL)
    free(buffer);

  return ok;
}

BOOL
meter_context_refresh_system_title(
    meter_context_t *self,
    const ber_type_t *type)
{
  BOOL ok = FALSE;
  
  if (type->constructed && type->id == PDU_TYPE_CALLED_AP_INVOCATION_ID) {
    if (type->field_count != 1) {
      ok = TRUE;
      goto fail;
    }
  
    /* Extract first element */
    type = type->field_list[0];
  
    if (type->ci != BER_CI_OCTET_STRING || type->bytecount != 8) {
      ok = TRUE;
      goto fail;
    }
      
    /* FOUND IT! */
    self->have_model = TRUE;
    memcpy(self->model, type->bytebuf, 3);
    memcpy(self->serial, type->bytebuf + 3, 5);
  }
    
  ok = TRUE;    
  
fail:
  return ok;    
}

BOOL
dlms_process_aarq(
    meter_context_t *self,
    ber_stream_t *stream,
    dlms_message_t **omsg)
{
  BOOL ok = FALSE;
  dlms_message_t *message = NULL;
  ber_type_t *type = NULL;
  uint8_t cmd;
  uint8_t len;

  *omsg = NULL;
  
  TRY(ber_stream_read_uint8(stream, cmd));
  TRY(ber_stream_read_uint8(stream, len));

  if (len > ber_stream_avail(stream))
    goto fail;

  CONSTRUCT(message, dlms_message, cmd, 0);
  
  while (!ber_stream_overflow(stream) && ber_stream_avail(stream) >= 2) {
    if ((type = ber_type_from_stream(stream)) != NULL) {
      if (cmd == DLMS_COMMAND_AARE)
        TRY(meter_context_refresh_system_title(self, type));
      TRY(dlms_message_give_type(message, type));
      type = NULL;
    }
  }

  /* Transfer message */
  *omsg = message;
  message = NULL;
  
  ok = TRUE;

fail:
  if (type != NULL)
    ber_type_destroy(type);

  if (message != NULL)
    dlms_message_destroy(message);

  return ok;
}

BOOL
dlms_process_pdu(
    meter_context_t *ctx,
    BOOL downstream,
    const uint8_t *data,
    size_t size,
    dlms_message_t **omsg)
{
  DLMS_COMMAND cmd;
  ber_stream_t *stream = NULL;
  struct dlms_callbacks *cb =
      (struct dlms_callbacks *) prime13_layer_get_userdata(
          meter_context_get_layer(ctx));
  BOOL ok = FALSE;

  if (size < 3) {
    ok = TRUE;
    goto fail;
  }

  cmd = data[0];

  if (cb->on_frame != NULL)
    TRY((cb->on_frame) (ctx, downstream, data, size, cb->userdata));

  TRY(stream = ber_stream_copy(data, size));

  switch (cmd) {
    case DLMS_COMMAND_AARE:
    case DLMS_COMMAND_AARQ:
      (void) dlms_process_aarq(ctx, stream, omsg);
      if (cb->on_meter != NULL)
        TRY((cb->on_meter) (ctx, cb->userdata));

      break;

    case DLMS_COMMAND_GET_REQUEST:
      (void) dlms_process_get_request(ctx, stream, omsg);
      break;

    case DLMS_COMMAND_GET_RESPONSE:
      (void) dlms_process_get_response(ctx, stream, omsg);
      break;

    case DLMS_COMMAND_RELEASE_RESPONSE:
      meter_context_clear_requests(ctx);
      break;

    default:
      if (cb->on_unsupported != NULL)
        TRY((cb->on_unsupported) (
            ctx,
            downstream,
            data,
            size,
            cb->userdata));
  }

  ok = TRUE;

fail:
  if (stream != NULL)
    ber_stream_destroy(stream);

  return ok;
}

/*****************************************************************************/
static void *
dlms_on_prime_meter(
    prime13_layer_t *self,
    prime13_node_t *node)
{
  meter_context_t *ctx;
  
  struct dlms_callbacks *cb =
        (struct dlms_callbacks *) prime13_layer_get_userdata(self);

  ctx = meter_context_new(self, node);

  if (ctx != NULL && cb->on_meter != NULL)
    (void) (cb->on_meter) (ctx, cb->userdata);

  return ctx;
}

void ber_hexdump (const void *data, uint32_t size);

static BOOL
dlms_on_prime_data(
    prime13_layer_t *self,
    prime13_node_t *node,
    void *userdata,
    BOOL downstream,
    const void *data,
    size_t size)
{
  dlms_message_t *message = NULL;
  struct dlms_callbacks *cb =
      (struct dlms_callbacks *) prime13_layer_get_userdata(self);
  meter_context_t *ctx = (meter_context_t *) userdata;
  BOOL ok = FALSE;

  (void) node;

  if (size > 3) {
    TRY(dlms_process_pdu(ctx, downstream, data + 3, size - 3, &message));
    
    if (cb->on_pdu != NULL && message != NULL)
      TRY((cb->on_pdu) (ctx, downstream, message, cb->userdata));
  }

  ok = TRUE;

fail:
  if (message != NULL)
    dlms_message_destroy(message);
  
  return ok;
}

static BOOL
dlms_on_mac_frame(
    void *userdata,
    const uint8_t *sna,
    BOOL direction,
    const void *data,
    size_t size)
{
  struct dlms_callbacks *cb = (struct dlms_callbacks *) userdata;

  if (cb->on_mac_frame != NULL)
    return (cb->on_mac_frame) (sna, 6, direction, data, size, cb->userdata);

  return TRUE;
}

static BOOL
dlms_on_subnet(
    prime13_layer_t *self,
    const uint8_t *sna,
    uint64_t times,
    void *userdata)
{
  struct dlms_callbacks *cb = (struct dlms_callbacks *) userdata;

  (void) self;

  if (cb->on_subnet != NULL)
    return (cb->on_subnet) (sna, times, cb->userdata);

  return TRUE;
}

static void
dlms_on_prime_dispose(void *userdata)
{
  meter_context_destroy((meter_context_t *) userdata);
}

prime13_layer_t *
dlms_on_prime_new(
    const struct prime13_phy_adapter *adapter,
    const struct dlms_callbacks *dlmscallbacks)
{
  prime13_layer_t *result = NULL;
  struct prime13_callbacks callbacks = prime13_callbacks_INITIALIZER;
  struct dlms_callbacks *copy = NULL;

  TRY(copy = malloc(sizeof(struct dlms_callbacks)));
  memcpy(copy, dlmscallbacks, sizeof (struct dlms_callbacks));

  callbacks.userdata  = copy;
  callbacks.on_meter  = dlms_on_prime_meter;
  callbacks.on_data   = dlms_on_prime_data;
  callbacks.on_frame  = dlms_on_mac_frame;
  callbacks.on_subnet = dlms_on_subnet;
  callbacks.dispose   = dlms_on_prime_dispose;
  callbacks.destroy   = free;

  TRY(result = prime13_layer_new(adapter, &callbacks));
  copy = NULL;

fail:
  if (copy != NULL)
    free(copy);

  return result;
}

