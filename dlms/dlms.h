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

#ifndef DLMS_DLMS_H
#define DLMS_DLMS_H

#include <dlms/dlmsmsg.h>
#include <dlms/meter.h>
#include <ber/ber.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct dlms_callbacks {
  void *userdata;
  BOOL (*on_mac_frame) (
      const uint8_t *mac,
      size_t maclen,
      BOOL direction,
      const uint8_t *data,
      size_t size,
      void *userdata);

  BOOL (*on_meter) (meter_context_t *meter, void *userdata);

  BOOL (*on_frame) (
      meter_context_t *meter,
      BOOL downstream,
      const uint8_t *data,
      size_t size,
      void *userdata);

  BOOL (*on_pdu) (
      meter_context_t *meter,
      BOOL downstream,
      dlms_message_t *message,
      void *userdata);

  BOOL (*on_subnet) (
      const uint8_t *sna,
      uint64_t times,
      void *userdata);

  BOOL (*on_unsupported) (
        meter_context_t *meter,
        BOOL downstream,
        const uint8_t *data,
        size_t size,
        void *userdata);

};

#define dlms_callbacks_INITIALIZER \
{ \
  NULL, /* userdata */ \
  NULL, /* on_frame */ \
  NULL, /* on_pdu */ \
  NULL, /* on_unsupported */ \
}

prime13_layer_t *dlms_on_prime_new(
    const struct prime13_phy_adapter *adapter,
    const struct dlms_callbacks *callbacks);

BOOL prime13_layer_configure_sqlite3_adapter(
    struct prime13_phy_adapter *adapter,
    const char *path);

BOOL prime13_layer_configure_serial_adapter(
    struct prime13_phy_adapter *adapter,
    const char *path,
    unsigned int baudrate);

void dlms_debug_layer(const prime13_layer_t *layer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DLMS_H */
