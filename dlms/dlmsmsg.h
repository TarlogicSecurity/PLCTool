/*
 * dlmsmsg.h: DLMS Message Objects
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef DLMS_DLMSMSG_H
#define DLMS_DLMSMSG_H

#include <ber/ber.h>
#include "PRIME/libprime/prime.h"
#include <enums.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define DLMS_CMD_OBIS_LENGTH 6

struct dlms_cmd_header {
  DLMS_COMMAND cmd;
  uint8_t invokeId;
};
  
struct dlms_get_request {
  uint8_t kind;
  uint8_t status;
  uint8_t obis[DLMS_CMD_OBIS_LENGTH];
  
  uint8_t access;
  PTR_LIST(ber_type_t, arg);
};

struct dlms_get_response {
  uint8_t kind;
  uint8_t status;
  ber_type_t *response;
  PTR_LIST(ber_type_t, item);
};

struct dlms_aarq {
  PTR_LIST(ber_type_t, entry);
};

struct dlms_aare {
  PTR_LIST(ber_type_t, entry);
};

struct dlms_message {
  struct dlms_cmd_header header;
  union {
    struct dlms_get_request get_request;
    struct dlms_get_response get_response;
    struct dlms_aarq aarq;
    struct dlms_aare aare;
  };      
};

typedef struct dlms_message dlms_message_t;

void dlms_message_destroy(dlms_message_t *self);
BOOL dlms_message_give_type(dlms_message_t *self, ber_type_t *type);
dlms_message_t *dlms_message_new(DLMS_COMMAND cmd, uint8_t invokeId);

const char *dlms_tag_to_pdu_type_str(uint8_t tag);
const char *dlms_authentication_to_string(unsigned int type);
const char *dlms_command_to_str(DLMS_COMMAND cmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DLMSMSG_H_ */
