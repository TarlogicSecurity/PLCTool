/*
 * spip.h: Serial Packet Injection Protocol
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef SPIP_SPIP_H_
#define SPIP_SPIP_H_

#include <stdint.h>
#include <util/defs.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !defined(__SAM4CMS16C_0__)
#  define gnu_printf
#endif /* !defined(__SAM4CMS16C_0__) */

#define SPIP_FRAME_SYNC (((uint64_t) 0xa5a5a5a5ul << (uint64_t) 32) | (uint64_t) 0xdeadcefeul)
#define SPIP_HEADER_SIZE 20

enum spip_command {
  SPIP_COMMAND_FRAME,
  SPIP_COMMAND_LED_SET_MASK,
  SPIP_COMMAND_LED_TOGGLE_MASK,
  SPIP_COMMAND_LCD  
};

struct spip_pdu {
  uint64_t sync;
  uint8_t command;
  uint8_t pad;
  uint16_t size;
  uint32_t header_crc;
  uint32_t crc;
  uint8_t data[0];
};

enum spip_loop_state {
  SPIP_LOOP_STATE_SYNCING,
  SPIP_LOOP_READING_HEADER,
  SPIP_LOOP_READING_BODY
};

struct spip_ctx {
  void *alloc_buf;
  size_t alloc_size;
  union {
    uint8_t sync[8];
    uint8_t header[0];
    struct spip_pdu pdu;
  };

  uint8_t p;
};

typedef struct spip_ctx spip_ctx_t;

#define spip_ctx_INITIALIZER \
{                            \
  NULL,                      \
  0,                         \
  { { 0 } },                 \
  0,                         \
}

struct spip_interface {
  void *userdata;
  BOOL (*write) (void *, const void *, size_t);
  BOOL (*write_byte) (void *, uint8_t c);
  BOOL (*read_byte) (void *, uint8_t *c);
  BOOL (*close) (void *);

  spip_ctx_t spip_ctx;
};

typedef struct spip_interface spip_iface_t;

#define spip_iface_INITIALIZER \
{                              \
  NULL,                        \
  NULL,                        \
  NULL,                        \
  NULL,                        \
  NULL,                        \
  spip_ctx_INITIALIZER,        \
}

/* Low level I/O methods */
BOOL spip_iface_write(spip_iface_t *iface, const struct spip_pdu *pdu);
BOOL spip_iface_read(spip_iface_t *iface, struct spip_pdu **pdu);

void spip_iface_dispose(spip_iface_t *iface, struct spip_pdu *pdu);
BOOL spip_iface_loop(
    spip_iface_t *iface,
    BOOL (*on_pdu) (spip_iface_t *iface, struct spip_pdu *pdu, void *userdata),
    void *userdata);
BOOL spip_iface_close(spip_iface_t *iface);

struct spip_pdu *spip_pdu_dup(const struct spip_pdu *pdu);

uint32_t spip_crc32b(const uint8_t *sna, const uint8_t *message, size_t len);

/* Helper functions */
BOOL spip_iface_write_frame(spip_iface_t *iface, const void *data, size_t size);
BOOL spip_iface_toggle_led_mask(spip_iface_t *iface, uint8_t mask);
BOOL spip_iface_set_led_mask(spip_iface_t *iface, uint8_t mask);
BOOL spip_iface_set_lcd(spip_iface_t *iface, uint8_t row, const char *text);
BOOL spip_iface_lcd_printf(
    spip_iface_t *iface,
    uint8_t row,
    const char *fmt,
    ...) __attribute__((gnu_printf));

/* System-specific methods */
BOOL spip_interface_open_serial(
    spip_iface_t *iface,
    const char *path,
    unsigned int baud);
BOOL spip_interface_open_pipe(spip_iface_t *iface);
BOOL spip_uart_interface_init(spip_iface_t *iface);

struct tx_task;

BOOL spip_iface_emulator_loop(spip_iface_t *iface);
BOOL spip_iface_board_loop(spip_iface_t *iface, struct tx_task *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SPIP_SPIP_H_ */
