/*
 * dlms.h: DLMS layer analizer
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

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
