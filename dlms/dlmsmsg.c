/*
 * dlmsmsg.c: DLMS Message Objects
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#define logfunc(severity, fmt, arg...)

#include "dlmsmsg.h"
#include <enums.h>

#define PDUTYPE(type) \
case JOIN(PDU_TYPE_, type): \
typeStr = STRINGIFY(type); \
break;

const char *
dlms_tag_to_pdu_type_str(uint8_t tag)
{
  const char *typeStr = "UNKNOWN_PDU";
  tag &= 0x1f;

  switch (tag) {
    PDUTYPE(PROTOCOL_VERSION);
    PDUTYPE(APPLICATION_CONTEXT_NAME);
    PDUTYPE(CALLED_AP_TITLE);
    PDUTYPE(CALLED_AE_QUALIFIER);
    PDUTYPE(CALLED_AP_INVOCATION_ID);
    PDUTYPE(CALLED_AE_INVOCATION_ID);
    PDUTYPE(CALLING_AP_TITLE);
    PDUTYPE(CALLING_AE_QUALIFIER);
    PDUTYPE(CALLING_AP_INVOCATION_ID);
    PDUTYPE(CALLING_AE_INVOCATION_ID);
    PDUTYPE(SENDER_ACSE_REQUIREMENTS);
    PDUTYPE(MECHANISM_NAME);
    PDUTYPE(CALLING_AUTHENTICATION_VALUE);
    PDUTYPE(IMPLEMENTATION_INFORMATION);
    PDUTYPE(USER_INFORMATION);
  }

  return typeStr;
}

const char *
dlms_authentication_to_string(unsigned int type)
{
  const char *str = "UNKNOWN";

  #define DLMSAUTH(type) \
  case JOIN(DLMS_AUTHENTICATION_, type): \
  str = STRINGIFY(type); \
  break;

  switch (type) {
    DLMSAUTH(NONE);
    DLMSAUTH(LOW);
    DLMSAUTH(HIGH);
    DLMSAUTH(HIGH_MD5);
    DLMSAUTH(HIGH_SHA1);
    DLMSAUTH(HIGH_SHA256);
    DLMSAUTH(HIGH_GMAC);
  }

  return str;
}

const char *
dlms_command_to_str(DLMS_COMMAND cmd)
{
  const char *cmdStr = "UNKNOWN";

  #define DLMSCMD(cmd) \
  case JOIN(DLMS_COMMAND_, cmd): \
  cmdStr = STRINGIFY(cmd); \
  break;

  switch (cmd) {
    DLMSCMD(NONE);
    DLMSCMD(INITIATE_REQUEST);
    DLMSCMD(INITIATE_RESPONSE);
    DLMSCMD(READ_REQUEST);
    DLMSCMD(READ_RESPONSE);
    DLMSCMD(WRITE_REQUEST);
    DLMSCMD(WRITE_RESPONSE);
    DLMSCMD(GET_REQUEST);
    DLMSCMD(GET_RESPONSE);
    DLMSCMD(SET_REQUEST);
    DLMSCMD(SET_RESPONSE);
    DLMSCMD(METHOD_REQUEST);
    DLMSCMD(METHOD_RESPONSE);
    DLMSCMD(DISCONNECT_MODE);
    DLMSCMD(UNACCEPTABLE_FRAME);
    DLMSCMD(SNRM);
    DLMSCMD(UA);
    DLMSCMD(AARQ);
    DLMSCMD(AARE);
    DLMSCMD(DISCONNECT_REQUEST);
    DLMSCMD(RELEASE_REQUEST);
    DLMSCMD(RELEASE_RESPONSE);
    DLMSCMD(CONFIRMED_SERVICE_ERROR);
    DLMSCMD(EXCEPTION_RESPONSE);
    DLMSCMD(GENERAL_BLOCK_TRANSFER);
    DLMSCMD(ACCESS_REQUEST);
    DLMSCMD(ACCESS_RESPONSE);
    DLMSCMD(DATA_NOTIFICATION);
    DLMSCMD(EVENT_NOTIFICATION);
    DLMSCMD(INFORMATION_REPORT);
    DLMSCMD(GLO_GET_REQUEST);
    DLMSCMD(GLO_GET_RESPONSE);
    DLMSCMD(GLO_SET_REQUEST);
    DLMSCMD(GLO_SET_RESPONSE);
    DLMSCMD(GLO_EVENT_NOTIFICATION_REQUEST);
    DLMSCMD(GLO_METHOD_REQUEST);
    DLMSCMD(GLO_METHOD_RESPONSE);
    DLMSCMD(GLO_INITIATE_REQUEST);
    DLMSCMD(GLO_READ_REQUEST);
    DLMSCMD(GLO_WRITE_REQUEST);
    DLMSCMD(GLO_INITIATE_RESPONSE);
    DLMSCMD(GLO_READ_RESPONSE);
    DLMSCMD(GLO_WRITE_RESPONSE);
    DLMSCMD(GENERAL_GLO_CIPHERING);
    DLMSCMD(GENERAL_DED_CIPHERING);
    DLMSCMD(GENERAL_CIPHERING);
    DLMSCMD(DED_GET_REQUEST);
    DLMSCMD(DED_GET_RESPONSE);
    DLMSCMD(DED_SET_REQUEST);
    DLMSCMD(DED_SET_RESPONSE);
    DLMSCMD(DED_EVENT_NOTIFICATION);
    DLMSCMD(DED_METHOD_REQUEST);
    DLMSCMD(DED_METHOD_RESPONSE);
    DLMSCMD(DED_READ_REQUEST);
    DLMSCMD(DED_WRITE_REQUEST);
    DLMSCMD(DED_READ_RESPONSE);
    DLMSCMD(DED_WRITE_RESPONSE);
    DLMSCMD(DED_INITIATE_REQUEST);
    DLMSCMD(DED_INITIATE_RESPONSE);
    DLMSCMD(GATEWAY_REQUEST);
    DLMSCMD(GATEWAY_RESPONSE);
  }

  return cmdStr;
}

void
dlms_message_destroy(dlms_message_t *self)
{
  int i;
  
  switch (self->header.cmd) {
    case DLMS_COMMAND_GET_REQUEST:
      for (i = 0; i < self->get_request.arg_count; ++i)
        if (self->get_request.arg_list[i] != NULL)
          ber_type_destroy(self->get_request.arg_list[i]);
        
      if (self->get_request.arg_list != NULL)
        free(self->get_request.arg_list);
      break;
    
    case DLMS_COMMAND_GET_RESPONSE:
      for (i = 0; i < self->get_response.item_count; ++i)
        if (self->get_response.item_list[i] != NULL)
          ber_type_destroy(self->get_response.item_list[i]);
    
      if (self->get_response.item_list != NULL)
        free(self->get_response.item_list);
      break;     
    
    case DLMS_COMMAND_AARE:
      for (i = 0; i < self->aare.entry_count; ++i)
        if (self->aare.entry_list[i] != NULL)
          ber_type_destroy(self->aare.entry_list[i]);
        
      if (self->aare.entry_list != NULL)
        free(self->aare.entry_list);
      break;
    
    case DLMS_COMMAND_AARQ:
      for (i = 0; i < self->aarq.entry_count; ++i)
        if (self->aarq.entry_list[i] != NULL)
          ber_type_destroy(self->aarq.entry_list[i]);
    
      if (self->aarq.entry_list != NULL)
        free(self->aarq.entry_list);
      break;
      
    default:
      i = 0; /* stfu */
  }
  
  free(self);
}

BOOL
dlms_message_give_type(dlms_message_t *self, ber_type_t *type)
{
  BOOL ok = FALSE;
  
  switch (self->header.cmd) {
    case DLMS_COMMAND_GET_REQUEST:
      TRYC(PTR_LIST_APPEND_CHECK(self->get_request.arg, type));
      break;
      
    case DLMS_COMMAND_GET_RESPONSE:
      TRYC(PTR_LIST_APPEND_CHECK(self->get_response.item, type));
      break;
      
    case DLMS_COMMAND_AARE:
      TRYC(PTR_LIST_APPEND_CHECK(self->aare.entry, type));
      break;
      
    case DLMS_COMMAND_AARQ:
      TRYC(PTR_LIST_APPEND_CHECK(self->aarq.entry, type));
      break;
      
    default:
      /* Wtf man */
      goto fail;    
  }
  
  ok = TRUE;
  
fail:  
  return ok;
}

dlms_message_t *
dlms_message_new(DLMS_COMMAND cmd, uint8_t invokeId)
{
  dlms_message_t *new = NULL;
  dlms_message_t *result = NULL;
  
  ALLOCATE(new, dlms_message_t);
  
  new->header.cmd = cmd;
  new->header.invokeId = invokeId;
  
  result = new;
  new = NULL;
  
fail:  
  return result;
}
