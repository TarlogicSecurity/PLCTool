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
