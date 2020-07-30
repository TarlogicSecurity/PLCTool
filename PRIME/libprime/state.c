/*
 * state.c: PRIME 1.3 State reconstruction
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#include "state.h"
#include <string.h>

static uint32_t crc32_table[] = {
        0x0,        0x4c11db7,  0x9823b6e,  0xd4326d9,
        0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
        0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
        0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
        0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
        0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
        0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
        0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
        0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
        0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
        0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
        0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
        0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
        0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
        0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
        0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
        0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
        0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
        0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
        0x18aeb13,  0x54bf6a4,  0x808d07d,  0xcc9cdca,
        0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
        0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
        0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
        0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
        0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
        0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
        0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
        0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
        0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
        0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
        0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
        0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
        0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
        0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
        0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
        0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
        0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
        0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
        0x315d626,  0x7d4cb91,  0xa97ed48,  0xe56f0ff,
        0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
        0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
        0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
        0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
        0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
        0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
        0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
        0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
        0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
        0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
        0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
        0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
        0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
        0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
        0x29f3d35,  0x65e2082,  0xb1d065b,  0xfdc1bec,
        0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
        0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
        0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
        0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
        0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
        0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
        0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
        0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
        0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
        0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4,
};

static const uint8_t crc8_table[256] = {
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38,
        0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D, 0x70, 0x77,
        0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46,
        0x41, 0x54, 0x53, 0x5A, 0x5D, 0xE0, 0xE7, 0xEE, 0xE9,
        0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4,
        0xC3, 0xCA, 0xCD, 0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B,
        0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA,
        0xBD, 0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA, 0xB7,
        0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88,
        0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A, 0x27, 0x20, 0x29,
        0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16,
        0x03, 0x04, 0x0D, 0x0A, 0x57, 0x50, 0x59, 0x5E, 0x4B,
        0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74,
        0x7D, 0x7A, 0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B,
        0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1,
        0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4, 0x69, 0x6E,
        0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F,
        0x58, 0x4D, 0x4A, 0x43, 0x44, 0x19, 0x1E, 0x17, 0x10,
        0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D,
        0x3A, 0x33, 0x34, 0x4E, 0x49, 0x40, 0x47, 0x52, 0x55,
        0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64,
        0x63, 0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, 0xAE,
        0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91,
        0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83, 0xDE, 0xD9, 0xD0,
        0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF,
        0xFA, 0xFD, 0xF4, 0xF3
};

/****************************** Fragment API **********************************/
void
prime13_fragmented_pdu_set_count(
    prime13_fragmented_pdu_t *self,
    unsigned int count)
{
  self->count = count;
}

void
prime13_fragmented_pdu_reset(prime13_fragmented_pdu_t *self)
{
  self->last = -1;

  growbuf_reset(&self->buffer);
}

BOOL
prime13_fragmented_pdu_copy(
    prime13_fragmented_pdu_t *dest,
    const prime13_fragmented_pdu_t *orig)
{
  struct growbuf bufcopy = growbuf_INITIALIZER;
  void *bufaddr;
  BOOL ok = FALSE;

  if (orig->buffer.size > 0) {
    TRY(bufaddr = growbuf_alloc(&bufcopy, orig->buffer.size));
    memcpy(bufaddr, orig->buffer.base, orig->buffer.size);
  }

  /* Allocation done, ready to copy */
  prime13_fragmented_pdu_reset(dest);
  dest->buffer = bufcopy;
  dest->count = orig->count;
  dest->last = orig->last;

  /* Set to zero (don't free) */
  growbuf_init(&bufcopy);
  ok = TRUE;

fail:
  growbuf_reset(&bufcopy);
  return ok;
}

BOOL
prime13_fragmented_pdu_put(
    prime13_fragmented_pdu_t *self,
    unsigned int index,
    const uint8_t *bytes,
    size_t size)
{
  void *buf = NULL;
  BOOL ok = FALSE;

  if (index - self->last != 1)
    goto fail;

  TRY(buf = growbuf_alloc(&self->buffer, size));
  memcpy(buf, bytes, size);

  self->last = index;

  ok = TRUE;

fail:
  return ok;
}

/****************************** Node API **************************************/
prime13_node_t *
prime13_node_new(const prime13_beacon_record_t *subnet, prime13_nid_t nid)
{
  prime13_node_t *new = NULL;

  ALLOCATE(new, prime13_node_t);

  new->subnet = subnet;
  new->nid = nid;
  new->is_new = TRUE;
  new->mac = PRIME13_MAC_UNRESOLVED;

  return new;

fail:
  if (new != NULL)
    prime13_node_destroy(new);

  return NULL;
}


prime13_node_t *
prime13_node_dup(const prime13_node_t *self)
{
  prime13_node_t *new = NULL;

  CONSTRUCT(new, prime13_node, self->subnet, self->nid);

  new->mac = self->mac;
  new->subnet = self->subnet;
  
  TRY(prime13_fragmented_pdu_copy(&new->down, &self->down));
  TRY(prime13_fragmented_pdu_copy(&new->up, &self->up));

  return new;

fail:
  if (new != NULL)
    prime13_node_destroy(new);

  return NULL;
}

void
prime13_node_set_fragment_count(
    prime13_node_t *self,
    BOOL downstream,
    unsigned int count)
{
  prime13_fragmented_pdu_t *frag = downstream ? &self->down : &self->up;

  prime13_fragmented_pdu_reset(frag);
  prime13_fragmented_pdu_set_count(frag, count);
}

BOOL
prime13_node_add_fragment(
    prime13_node_t *self,
    BOOL downstream,
    int index,
    const uint8_t *bytes,
    size_t fraglen)
{
  prime13_fragmented_pdu_t *frag = downstream ? &self->down : &self->up;
  BOOL ok = FALSE;

  if (index == -1)
    index = frag->count - 1;

  if (!prime13_fragmented_pdu_put(frag, index, bytes, fraglen))
    goto fail;

  /* Last fragment added */
  if (frag->count - frag->last == 1)
    ok = TRUE;

fail:
  return ok;
}

BOOL
prime13_node_assemble(
    prime13_node_t *self,
    BOOL downstream,
    const uint8_t **bytes,
    size_t *size)
{
  prime13_fragmented_pdu_t *frag = downstream ? &self->down : &self->up;

  if (frag->count != frag->last + 1)
    return FALSE;

  *bytes = frag->buffer.as_bytes;
  *size  = frag->buffer.size;

  return TRUE;
}

void
prime13_node_destroy(prime13_node_t *self)
{
  prime13_fragmented_pdu_reset(&self->down);
  prime13_fragmented_pdu_reset(&self->up);

  free(self);
}

/***************************** PRIME Beacon Record ****************************/
void
prime13_beacon_record_destroy(prime13_beacon_record_t *self)
{
  free(self);
}

void
prime13_beacon_record_visit(prime13_beacon_record_t *self)
{
  ++self->count;
  time(&self->last);
}

static inline BOOL
prime13_beacon_record_compare(
    const prime13_beacon_record_t *self,
    const uint8_t *sna)
{
  return memcmp(self->sna, sna, 6) == 0;
}

prime13_beacon_record_t *
prime13_beacon_record_new(const uint8_t *sna)
{
  prime13_beacon_record_t *new = NULL;
  prime13_beacon_record_t *result = NULL;

  ALLOCATE(new, prime13_beacon_record_t);

  memcpy(new->sna, sna, 6);

  result = new;
  new = NULL;

fail:
  if (new != NULL)
    prime13_beacon_record_destroy(new);

  return result;
}

/******************************* PRIME State **********************************/
prime13_state_t *
prime13_state_new(void)
{
  prime13_state_t *new = NULL;

  ALLOCATE(new, prime13_state_t);

  return new;

fail:
  if (new != NULL)
    prime13_state_destroy(new);

  return NULL;
}

prime13_beacon_record_t *
prime13_state_lookup_beacon_record(
    const prime13_state_t *self,
    const uint8_t *sna)
{
  int i;

  for (i = 0; i < self->beacon_count; ++i)
    if (prime13_beacon_record_compare(self->beacon_list[i], sna))
      return self->beacon_list[i];

  return NULL;
}

prime13_node_t *
prime13_state_lookup_node(
    const prime13_state_t *self,
    const prime13_beacon_record_t *subnet,
    prime13_nid_t nid)
{
  int i;

  for (i = 0; i < self->node_count; ++i)
    if (self->node_list[i]->nid == nid 
      && self->node_list[i]->subnet == subnet)
      return self->node_list[i];

  return NULL;
}

prime13_beacon_record_t *
prime13_state_assert_beacon_record(
    prime13_state_t *self,
    const uint8_t *sna)
{
  prime13_beacon_record_t *beacon = NULL;
  prime13_beacon_record_t *created = NULL;

  if ((beacon = prime13_state_lookup_beacon_record(self, sna)) == NULL) {
    CONSTRUCT(created, prime13_beacon_record, sna);
    TRYC(PTR_LIST_APPEND_CHECK(self->beacon, created));
    beacon = created;
    created = NULL;
  }

fail:
  if (created != NULL)
    prime13_beacon_record_destroy(created);

  return beacon;
}

prime13_node_t *
prime13_state_assert_node(
    prime13_state_t *self,
    const prime13_beacon_record_t *subnet,
    prime13_nid_t nid)
{
  prime13_node_t *node = NULL;
  prime13_node_t *created = NULL;

  if ((node = prime13_state_lookup_node(self, subnet, nid)) == NULL) {
    CONSTRUCT(created, prime13_node, subnet, nid);
    TRYC(PTR_LIST_APPEND_CHECK(self->node, created));
    node = created;
    created = NULL;
  }

fail:
  if (created != NULL)
    prime13_node_destroy(created);

  return node;
}

prime13_state_t *
prime13_state_dup(const prime13_state_t *self)
{
  prime13_state_t *new = NULL;
  prime13_node_t *node = NULL;
  int i;

  CONSTRUCT(new, prime13_state);

  /* Duplicate all nodes */
  for (i = 0; i < self->node_count; ++i) {
    TRY(node = prime13_state_assert_node(
      new, 
      self->node_list[i]->subnet,
      self->node_list[i]->nid));
    node->mac = self->node_list[i]->mac;
  }

  return new;

fail:
  if (new != NULL)
    prime13_state_destroy(new);

  return NULL;
}

void
prime13_state_destroy(prime13_state_t *state)
{
  int i;

  for (i = 0; i < state->node_count; ++i)
    prime13_node_destroy(state->node_list[i]);

  free(state);
}

#define LEN_CONST_BCN_PRIME_1_4       4
static const uint8_t puc_crc_const_1_4[LEN_CONST_BCN_PRIME_1_4] = {0x00, 0x01, 0x04, 0x00};

static inline uint32_t
prime13_crc32_step(uint32_t ul_crc_init, const uint8_t *puc_buf_ptr, uint32_t ul_len)
{
  uint8_t uc_idx;
  uint32_t ul_crc;

  ul_crc = ul_crc_init;
  if (ul_len != 0) {
    while (ul_len--) {
      uc_idx = (uint8_t)(ul_crc >> 24) ^ *puc_buf_ptr++;
      ul_crc = (ul_crc << 8) ^ crc32_table[uc_idx];
    }
  }

  return ul_crc;
}

static inline uint8_t
prime13_crc8_step(uint32_t ul_crc_init, const uint8_t *puc_buf_ptr, uint32_t ul_len)
{
  uint8_t uc_crc;

  uc_crc = ul_crc_init;

  while (ul_len--)
    uc_crc = crc8_table[uc_crc ^ *puc_buf_ptr++];

  return uc_crc;
}

uint8_t
prime13_crc8b(const uint8_t *sna, const uint8_t *message, size_t len)
{
  uint8_t crc = 0;

  if (sna)
    crc = prime13_crc8_step(crc, sna, 6);

  crc = prime13_crc8_step(crc, message, len);

  return crc;
}

uint32_t
prime13_crc32b(const uint8_t *sna, const uint8_t *message, size_t len)
{
  uint32_t crc = 0;

  if (sna)
    crc = prime13_crc32_step(crc, sna, 6);

  crc = prime13_crc32_step(crc, message, len);
  return crc;
}

static BOOL
prime13_parse_data_packet(
    prime13_state_t *self,
    const prime13_beacon_record_t *sna,
    struct prime13_packet_hdr *pkt)
{
  BOOL ok = FALSE;
  size_t payload_len = pkt->len;
  struct prime13_sar_hdr *sar;
  prime13_nid_t nid;
  const uint8_t *bytes = pkt->payload;
  prime13_node_t *node = NULL;
  BOOL downstream = pkt->mac.du;
  unsigned int i;

  i = 0;

  /* Compute NID */
  nid = PRIME13_SID_LNID_TO_NID(pkt->sid, pkt->lnid);
  TRY(node = prime13_state_assert_node(self, sna, nid));
  self->pdu_info.direction = downstream;
  self->pdu_info.data = TRUE;
  self->pdu_info.node = node;

  /* Skip ARQ header */
  do {  } while (i < payload_len && pkt->arq[i++].m);

  bytes += i;
  payload_len -= i;

  if (payload_len > 0) {
    /* Parse SAR */
    sar = (struct prime13_sar_hdr *) bytes;
    /* Advance */
    ++bytes;
    --payload_len;

    switch (sar->type) {
      case 0:
        /* First segment */
        prime13_node_set_fragment_count(node, downstream, sar->field + 1);

        prime13_node_add_fragment(
            node,
            downstream,
            0,
            bytes,
            payload_len);

        break;

      case 1:
        /* Intermediate segment */
        prime13_node_add_fragment(
            node,
            downstream,
            sar->field + 1,
            bytes,
            payload_len);
        break;

      case 2:
        /* Last segment */
        prime13_node_add_fragment(
            node,
            downstream,
            -1,
            bytes,
            payload_len);

      /*default:
        printf("Invalid SAR type %d\n", sar->type); */
    }
  }

  ok = TRUE;

fail:
  return ok;
}

static inline const char *
prime13_ctype_to_string(uint16_t ctype)
{
  const char *strs[] = {
      "DAT",
      "REG", "CON", "PRO", "BSI", "FRA",
      "CFP", "ALV", "MUL", "PRM", "SEC"};

  if (ctype > 10)
    return "UNKNOWN";

  return strs[ctype];
}

static BOOL
prime13_parse_control_packet(
    prime13_state_t *state,
    const prime13_beacon_record_t *sna,
    struct prime13_packet_hdr *pkt)
{
  BOOL ok = FALSE;
  prime13_nid_t nid;
  prime13_node_t *node = NULL;
  BOOL downstream = pkt->mac.du;
  
  char timestr[32];
  char *p;

  nid = PRIME13_SID_LNID_TO_NID(pkt->sid, pkt->lnid);
  TRY(node = prime13_state_assert_node(state, sna, nid));
  state->pdu_info.direction = downstream;
  state->pdu_info.node = node;
  state->pdu_info.data = FALSE;
  
  strncpy(timestr, ctime(&state->received), sizeof(timestr));
  timestr[sizeof(timestr) - 1] = '\0';

  if ((p = strchr(timestr, '\n')) != NULL)
    *p = '\0';
/*
  fprintf(
      stderr,
      "\033[2K%s PDU (%d bytes) - %lld PDUs received (last: %s)\r",
      prime13_ctype_to_string(pkt->lcid_ctype),
      pkt->len,
      state->counter,
      timestr);
*/
  ok = TRUE;

fail:
  return ok;
}

BOOL
prime13_state_parse_beacon(
    prime13_state_t *state,
    struct prime13_beacon_pdu_hdr *pkt)
{
  BOOL ok = FALSE;
  prime13_beacon_record_t *record;
  
  TRY(record = prime13_state_assert_beacon_record(state, pkt->sna));

  prime13_beacon_record_visit(record);

  state->beacon_record = record;

  ok = TRUE;

fail:
  return ok;
}

prime13_beacon_record_t *
prime13_state_guess_gpdu_sna(
    const prime13_state_t *self,
    const uint8_t *data,
    size_t size)
{
  int i;
  uint8_t crc8;
  uint32_t crc32;
  size_t truesize;

  if (size < sizeof(uint32_t))
    return NULL;

  truesize = size - sizeof(uint32_t);

  crc32 = *((uint32_t *) (data + truesize));
  crc8  = data[2];

  prime13_nob_swap(&crc32, sizeof(uint32_t));

  for (i = 0; i < self->beacon_count; ++i)
    if (prime13_crc8b(self->beacon_list[i]->sna, data, 2) == crc8
        && prime13_crc32b(self->beacon_list[i]->sna, data, truesize) == crc32)
      return self->beacon_list[i];

  return NULL;
}

BOOL
prime13_state_no_beacons(const prime13_state_t *self)
{
  return self->beacon_count == 0;
}

BOOL
prime13_state_feed(
    prime13_state_t *self,
    const void *data,
    size_t size)
{
  void *copy = NULL;
  prime13_beacon_record_t *sna;
  struct prime13_packet_hdr *pkt;
  struct prime13_beacon_pdu_hdr *beacon;
  struct prime13_generic_mac_hdr *mac;
  uint32_t pducrc;
  uint32_t crc32;
  BOOL ok = FALSE;

  self->pdu_info.node = NULL;
  self->beacon_record = NULL;

  ++self->counter;
  time(&self->received);

  if (size >= 4) {
    ALLOCATE_MANY(copy, size, uint8_t);
    memcpy(copy, data, size);

    mac = copy;

    switch (mac->ht) {
      case 0:
        pkt = copy;
        prime13_nob_swap(pkt->nob1, 2);
        prime13_nob_swap(pkt->nob2, 4);

        /* Sanity checks */
        if (size !=
            pkt->len + sizeof(struct prime13_packet_hdr) + sizeof(uint32_t))
          break;

        if ((sna = prime13_state_guess_gpdu_sna(self, data, size)) != NULL) {
          if (pkt->c) {
            TRY(prime13_parse_control_packet(self, sna, pkt));
          } else {
            TRY(prime13_parse_data_packet(self, sna, pkt));
          }
        }
        break;

      case 2:
        beacon = copy;
        crc32 = prime13_crc32b(
            NULL,
            (const uint8_t *) beacon,
            sizeof(struct prime13_beacon_pdu_hdr) - 4);

        pducrc = beacon->crc;
        prime13_nob_swap(&pducrc, 4);

        if (crc32 == pducrc) {
  /*        ber_hexdump(beacon, sizeof(struct prime13_beacon_pdu_hdr));
          prime13_nob_swap(beacon->nob, 2);
          printf("  QLTY: %02x\n", beacon->qlty);
          printf("  RESV: %02x\n", beacon->resv);
          printf("  HT:   %02x\n", beacon->ht);
          printf("  UNUS: %02x\n", beacon->unused);
          printf("  SID:  %02x\n", beacon->sid);
          printf("  CFP:  %04x\n", beacon->count);
          printf("  LVL:  %02x\n", beacon->level);
          printf("  RSV2: %02x\n", beacon->resv2);
          printf("  FREQ: %02x\n", beacon->freq);
          printf("  SEQ:  %02x\n", beacon->seq);
          printf("  SNA: %02x:%02x:%02x:%02x:%02x:%02x\n",
                 beacon->sna[0],
                 beacon->sna[1],
                 beacon->sna[2],
                 beacon->sna[3],
                 beacon->sna[4],
                 beacon->sna[5]);
          printf("UPCOST: %02x\n", beacon->upcost);
          printf("DNCOST: %02x\n", beacon->downcost);
*/
          TRY(prime13_state_parse_beacon(self, beacon));
        }

        break;
    }
  }


  ok = TRUE;

fail:
  if (copy != NULL)
    free(copy);

  return ok;
}

