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

#ifndef _PRIME_PDU_H
#define _PRIME_PDU_H

#ifdef CURRFILE
#undef CURRFILE
#endif /* CURRFILE */

#define CURRFILE PDU_H

#include <PLCTool/util/defs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct prime13_arq_ack {
  uint8_t ackid : 6;
  uint8_t flush : 1;
  uint8_t m : 1;
} PACKED;

struct prime13_arq_win {
  uint8_t winsize : 5;
  uint8_t res : 1;
  uint8_t flush : 1;
  uint8_t m : 1;
} PACKED;

struct prime13_arq_nack {
  uint8_t nackid : 6;
  uint8_t flush : 1;
  uint8_t m : 1;
} PACKED;

struct prime13_sar_hdr {
  uint8_t field : 6;
  uint8_t type : 2;
} PACKED;

CASSERT(sizeof(struct prime13_sar_hdr) == 1);

union prime13_arq_info {
  struct prime13_arq_ack ack;
  struct prime13_arq_win win;
  struct prime13_arq_nack nack;

  struct {
    uint8_t field : 6;
    uint8_t flush : 1;
    uint8_t m : 1;
  } PACKED;
} PACKED;

struct prime13_arq_header {
  uint8_t pktid : 6;
  uint8_t flush : 1;
  uint8_t m : 1;

  union prime13_arq_info info[0];
} PACKED;

struct prime13_generic_mac_hdr {
  uint8_t resv : 4;
  uint8_t ht : 2;
  uint8_t unused : 2;
  uint8_t level : 6;
  uint8_t du : 1;
  uint8_t resv2 : 1;
  uint8_t hcs;
};

struct prime13_reg_header {
  union {
    struct {
      uint8_t time : 3;
      uint8_t cap_arq : 1;
      uint8_t cap_prm : 1;
      uint8_t cap_mc : 1;
      uint8_t cap_dc : 1;
      uint8_t cap_cfp : 1;
      uint8_t cap_pa : 1;
      uint8_t cap_sw : 1;
      uint8_t resv : 2;
      uint8_t spc : 2;
      uint8_t r : 1;
      uint8_t n : 1;
    } PACKED;
    uint8_t nob1[2];
  };
  uint8_t eui48[6];
  struct {
    uint8_t snk[16];
    uint8_t auk[16];
  } keys[0];
} PACKED;

struct prime13_pro_header {
  struct {
    uint8_t time : 3;
    uint8_t rq : 3;
    uint8_t r : 1;
    uint8_t n : 1;
  } PACKED;
  uint8_t nsid;
  uint8_t pna[6];
  uint8_t upcost;
  uint8_t dncost;
  struct {
    uint8_t swc_arq : 1;
    uint8_t swc_prm : 1;
    uint8_t swc_mc : 1;
    uint8_t swc_dc : 1;
  } PACKED;
};

struct prime13_con_header {
  union {
    struct {
      uint16_t lcid : 9;
      uint8_t reserved : 3;
      uint8_t e : 1;
      uint8_t arq : 1;
      uint8_t d : 1;
      uint8_t n : 1;
    } PACKED;
    uint8_t nob1[2];
  };

  union {
    struct {
      uint8_t eui48[6];
    } PACKED eui48[0];

    struct {
      union {
        uint16_t dclcid : 9;
        uint8_t nob2[2];
      };

      union {
        struct {
          uint16_t dclnid : 14;
          uint8_t res : 1;
          uint8_t dcnad : 1;
        } PACKED;
        uint8_t nob3[2];
      };

      uint8_t dcsid;
      uint8_t dssid;
    } direct[0] PACKED;

    struct {
      uint8_t type;
      uint8_t dlen;
    } PACKED tail[0];
  };
} PACKED;

struct prime13_packet_hdr {
  struct prime13_generic_mac_hdr mac;
  union {
    struct {
      uint16_t lcid_ctype : 9;
      uint8_t c : 1;
      uint8_t prio : 2;
      uint8_t nad : 1;
      uint8_t reserved : 3;
    } PACKED;
    uint8_t nob1[2];
  };

  union {
    struct {
      uint16_t len : 9;
      uint8_t spad : 1;
      uint16_t lnid : 14;
      uint8_t sid;
    } PACKED;
    uint8_t nob2[4];
  };

  union {
    struct prime13_con_header con[0];
    struct prime13_reg_header reg[0];
    struct prime13_pro_header pro[0];
    struct prime13_arq_header arq[0];
    uint8_t payload[0];
  };
} PACKED;

struct prime13_beacon_pdu_hdr {
  uint8_t qlty : 3;
  uint8_t resv : 1;
  uint8_t ht : 2;
  uint8_t unused : 2;

  uint8_t sid;

  union {
    struct {
      uint16_t cfp : 10;
      uint16_t pos : 3;
      uint16_t count : 3;
    };
    uint8_t nob[2];
  };

  uint8_t level : 6;
  uint8_t resv2 : 2;
  uint8_t freq : 3;
  uint8_t seq : 5;
  uint8_t sna[6];
  uint8_t upcost;
  uint8_t downcost;
  uint32_t crc;
} PACKED;

static inline void
prime13_nob_swap(void *buf, size_t len)
{
#ifdef __cplusplus
  uint8_t *bytes = static_cast<uint8_t *>(buf);
#else
  uint8_t *bytes = (uint8_t *) buf;
#endif /* __cplusplus */
  size_t i, hlen;
  uint8_t tmp;

  hlen = len / 2;

  for (i = 0; i < hlen; ++i) {
    tmp = bytes[i];
    bytes[i] = bytes[len - i - 1];
    bytes[len - i - 1] = tmp;
  }
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRIME_PDU_H */
