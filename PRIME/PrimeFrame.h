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

#ifndef _PLCHACK_ADAPTERS_PRIMEFRAME_H
#define _PLCHACK_ADAPTERS_PRIMEFRAME_H

#include "libprime/prime.h"
#include <vector>
#include <string>
#include <Topology/Node.h>

namespace PLCTool {
  class PrimeFrame {
  public:
    enum FragmentType {
      FIRST        = 0,
      INTERMEDIATE = 1,
      LAST         = 2,
      RESERVED     = 3
    };

    struct HDR {
      uint8_t HT, LEVEL, DO, HCS;
    };

    struct CL {
      uint8_t TYPE;
      uint8_t SRC;
      uint8_t DEST;
    };

    struct SAR {
      FragmentType TYPE;
      union {
        uint8_t FIELD;
        uint8_t SEQ;
        uint8_t NSEGS;
      };
    };

    struct PKT {
      uint16_t LCID_CTYPE;
      bool     C;
      uint8_t  PRIO;
      bool     NAD;
      uint16_t LEN;
      uint16_t LNID;
      uint8_t  SID;
    };

    struct CON {
      bool N, D, ARQ, E, DCNAD;
      uint8_t DSSID, DCSID, TYPE, DLEN;
      uint8_t EUI_48[6];
      uint16_t LCID, DCLCID, DCLNID;
      std::vector<uint8_t> DATA;
    };

    struct ARQ {
      uint8_t PKTID;
      uint8_t WINSIZE;
      uint8_t ACKID;
      std::vector<uint8_t> NACKID;
    };

    struct REG {
      bool N, R, CAP_SW, CAP_PA, CAP_CFP, CAP_DC, CAP_MC, CAP_ARQ;
      uint8_t SPC;
      uint8_t TIME;
      uint8_t EUI_48[6];
      uint8_t SNK[16];
      uint8_t AUK[16];
    };

    struct PRO {
      bool N, SWC_DC, SWC_MC, SWC_PRM, SWC_ARQ;
      uint8_t RQ, TIME, NSID;
      uint8_t PNA[6];
      uint8_t UPCOST;
      uint8_t DNCOST;
    };

    struct BCN {
      uint8_t SID;
      uint8_t QLTY;
      uint8_t CFP;
      uint8_t POS;
      uint8_t CNT;
      uint8_t FRQ;
      uint8_t SEQ;
      uint8_t LEVEL;
      uint8_t SNA[6];
      uint8_t UPCOST;
      uint8_t DNCOST;
    };

    enum MACType {
      GENERIC,
      BEACON
    };

    enum GenericType {
      DATA = 0,
      REG = PRIME13_CTYPE_REG,
      CON = PRIME13_CTYPE_CON,
      PRO = PRIME13_CTYPE_PRO,
      BSI = PRIME13_CTYPE_BSI,
      FRA = PRIME13_CTYPE_FRA,
      CFP = PRIME13_CTYPE_CFP,
      ALV = PRIME13_CTYPE_ALV,
      MUL = PRIME13_CTYPE_MUL,
      PRM = PRIME13_CTYPE_PRM,
      SEC = PRIME13_CTYPE_SEC
    };

  private:
    std::vector<uint8_t> serializeDATA(void) const;
    bool deserializeDATA(const uint8_t *payload, size_t size);

    std::vector<uint8_t> serializeREG(void) const;
    bool deserializeREG(struct prime13_reg_header *header, size_t size);

    std::vector<uint8_t> serializePRO(void) const;
    bool deserializePRO(struct prime13_pro_header *header, size_t size);

    std::vector<uint8_t> serializeCON(void) const;
    bool deserializeCON(struct prime13_con_header *header, size_t size);

    std::vector<uint8_t> serializeGeneric(void) const;
    bool deserializeGeneric(struct prime13_packet_hdr *header);

    std::vector<uint8_t> serializeBeacon(void) const;
    bool deserializeBeacon(struct prime13_beacon_pdu_hdr *header);

    bool deserialize(const uint8_t *data, size_t size);

    void printCON(std::string &) const;
    void printDATA(std::string &) const;
    void printREG(std::string &) const;
    void printPRO(std::string &) const;
    void printGeneric(std::string &) const;
    void printBeacon(std::string &) const;

  public:
    uint8_t sna[6];
    struct PrimePDU {
      MACType macType = GENERIC;
      GenericType genType = GenericType::DATA;

      struct SAR SAR;
      struct HDR HDR;
      struct ARQ ARQ;
      struct CON CON;
      struct PKT PKT;
      struct REG REG;
      struct PRO PRO;
      struct BCN BCN;
      struct CL  CL;


      std::vector<uint8_t> DATA;
    } PDU;

    // Beacon frame
    PrimeFrame(const uint8_t *mac);

    // Generic control packet
    PrimeFrame(const uint8_t *mac, enum prime13_ctype type);

    // Prime data frame
    PrimeFrame(const uint8_t *mac, const uint8_t *data, size_t);
    PrimeFrame(const uint8_t *mac, std::vector<uint8_t> const &data);

    std::string toString() const;
    void print(FILE *ofp = stderr) const;

    std::string typeToString(void) const;

    std::vector<uint8_t> serialize(void) const;

    static PrimeFrame *fromRawData(
        const uint8_t *mac, std::vector<uint8_t> const &data);
    static PrimeFrame *fromRawData(
        const uint8_t *mac,
        const uint8_t *data,
        size_t size);

    static void printToString(std::string &dest, const char *fmt, ...);
    static void vprintToString(std::string &dest, const char *fmt, va_list ap);

    static bool parseSNA(uint8_t *, std::string const &);
    static bool parseNID(NodeId &id, std::string const &);
    static bool parseLCID(ConnId &id, std::string const &);
    static bool parseInteger(int &val, std::string const &);

    static void hexdump(std::string &, const void *data, size_t);
    static void hexdump(FILE *fp, const void *data, size_t size);
    static void hexdump(const void *data, size_t size);
  };
}

#endif // _PLCHACK_ADAPTERS_PRIMEFRAME
