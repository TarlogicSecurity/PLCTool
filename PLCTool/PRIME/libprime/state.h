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

#ifndef _PRIME_STATE_H
#define _PRIME_STATE_H

#include <time.h>

#include "pdu.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef uint32_t prime13_nid_t;
typedef uint64_t prime13_mac_t;

#define PRIME13_MAC_UNRESOLVED 0xffffffffffffffffull
#define PRIME13_MAC_IS_VALID(mac) ((mac) & 0xffff000000000000ull)

#define PRIME13_SID_LNID_TO_NID(sid, lnid) \
  ((sid) << 14) | ((lnid) & 0x3fff)

#define PRIME13_SID(nid) (0xff & ((nid) >> 14))
#define PRIME13_LNID(nid) (0x3fff & (nid))

enum prime13_ctype {
  PRIME13_CTYPE_DAT = 0,
  PRIME13_CTYPE_REG = 1,
  PRIME13_CTYPE_CON = 2,
  PRIME13_CTYPE_PRO = 3,
  PRIME13_CTYPE_BSI = 4,
  PRIME13_CTYPE_FRA = 5,
  PRIME13_CTYPE_CFP = 6,
  PRIME13_CTYPE_ALV = 7,
  PRIME13_CTYPE_MUL = 8,
  PRIME13_CTYPE_PRM = 9,
  PRIME13_CTYPE_SEC = 10
};

struct prime13_fragmented_pdu {
  unsigned int count;
  unsigned int last;
  struct growbuf buffer;
  void *ctx;
};

typedef struct prime13_fragmented_pdu prime13_fragmented_pdu_t;

void prime13_fragmented_pdu_set_count(
    prime13_fragmented_pdu_t *self,
    unsigned int count);
void prime13_fragmented_pdu_set_context(
    prime13_fragmented_pdu_t *self,
    void *ctx);
void prime13_fragmented_pdu_reset(prime13_fragmented_pdu_t *self);
BOOL prime13_fragmented_pdu_copy(
    prime13_fragmented_pdu_t *dest,
    const prime13_fragmented_pdu_t *orig);

BOOL prime13_fragmented_pdu_put(
    prime13_fragmented_pdu_t *dest,
    unsigned int index,
    const uint8_t *bytes,
    size_t size);

struct prime13_beacon_record {
  uint8_t sna[8];
  unsigned int count;
  time_t last;
};

typedef struct prime13_beacon_record prime13_beacon_record_t;

struct prime13_node {
  const prime13_beacon_record_t *subnet;
  prime13_nid_t nid;
  prime13_mac_t mac;

  prime13_fragmented_pdu_t up;
  prime13_fragmented_pdu_t down;

  void *userdata;
  BOOL is_new;
};

typedef struct prime13_node prime13_node_t;

void prime13_node_set_data_ephemereal_context(
    prime13_node_t *self,
    BOOL downstream,
    void *ctx);

prime13_node_t *prime13_node_new( 
  const prime13_beacon_record_t *subnet,
  prime13_nid_t nid);
  
prime13_node_t *prime13_node_dup(const prime13_node_t *self);

void prime13_node_set_fragment_count(
    prime13_node_t *self,
    BOOL downstream,
    unsigned int count);

BOOL prime13_node_add_fragment(
    prime13_node_t *self,
    BOOL downstream,
    int index,
    const uint8_t *bytes,
    size_t fraglen);
BOOL prime13_node_assemble(
    prime13_node_t *self,
    BOOL downstream,
    const uint8_t **bytes,
    size_t *size);

static inline BOOL
prime13_node_is_new(const prime13_node_t *self)
{
  return self->is_new;
}

static inline void
prime13_node_ack(prime13_node_t *self)
{
  self->is_new = FALSE;
}

void prime13_node_destroy(prime13_node_t *self);

struct prime13_state_pdu_info {
  prime13_node_t *node;
  BOOL data;
  BOOL direction;
};

struct prime13_state {
  uint64_t counter;
  time_t received;
  PTR_LIST(prime13_node_t, node);
  PTR_LIST(prime13_beacon_record_t, beacon);

  prime13_beacon_record_t *beacon_record;
  struct prime13_state_pdu_info pdu_info;
};

typedef struct prime13_state prime13_state_t;

static inline const struct prime13_state_pdu_info *
prime13_state_get_pdu_info(const prime13_state_t *self)
{
  if (self->pdu_info.node != NULL)
    return &self->pdu_info;

  return NULL;
}

static inline prime13_beacon_record_t *
prime13_state_get_beacon_record(const prime13_state_t *self)
{
  return self->beacon_record;
}

prime13_state_t *prime13_state_new(void);
prime13_state_t *prime13_state_dup(const prime13_state_t *state);
prime13_node_t *prime13_state_lookup_node(
    const prime13_state_t *self,
    const prime13_beacon_record_t *subnet,
    prime13_nid_t nid);
prime13_node_t *prime13_state_assert_node(
    prime13_state_t *self,
    const prime13_beacon_record_t *subnet,
    prime13_nid_t nid);

void *prime13_state_for_each_node(
    prime13_state_t *self,
    void *(*func) (
        prime13_state_t *self,
        prime13_node_t *node,
        void *context),
    void *context);

void prime13_state_destroy(prime13_state_t *state);
BOOL prime13_state_feed(
    prime13_state_t *state,
    const void *data,
    size_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRIME_STATE_H */
