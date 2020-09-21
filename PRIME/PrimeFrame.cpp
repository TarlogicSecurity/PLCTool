//
// PrimeFrame.cpp: PRIME 1.3 frame parser
//
// (c) 2019 Tarlogic Security S.L. - All rights reserved
//
// Company confidential. Any unauthorized use, disclosure, reproduction or
// distribution of this file is strictly prohibited.
//

#include "PrimeFrame.h"
#include <iostream>
#include <cstring>
#include <cstdarg>

using namespace PLCTool;

//////////////////////////// Frame deserialization /////////////////////////////
std::vector<uint8_t>
PrimeFrame::serializeREG(void) const
{
  size_t expectedSize;
  std::vector<uint8_t> pdu;
  struct prime13_reg_header *hdr;

  expectedSize =
      sizeof(struct prime13_reg_header) + 32 * (this->PDU.REG.SPC > 0);

  pdu.resize(expectedSize);

  hdr = reinterpret_cast<struct prime13_reg_header *>(pdu.data());

   hdr->n = this->PDU.REG.N;
   hdr->r = this->PDU.REG.R;
   hdr->cap_sw = this->PDU.REG.CAP_SW;
   hdr->cap_pa = this->PDU.REG.CAP_PA;
   hdr->cap_cfp = this->PDU.REG.CAP_CFP;
   hdr->cap_dc = this->PDU.REG.CAP_DC;
   hdr->cap_mc = this->PDU.REG.CAP_MC;
   hdr->cap_arq = this->PDU.REG.CAP_ARQ;
   hdr->spc = this->PDU.REG.SPC;
   hdr->time = this->PDU.REG.TIME;

  memcpy(hdr->eui48, this->PDU.REG.EUI_48, 6);

  if (this->PDU.REG.SPC > 0) {
    memcpy(hdr->keys[0].snk, this->PDU.REG.SNK, 16);
    memcpy(hdr->keys[0].auk, this->PDU.REG.AUK, 16);
  }

  prime13_nob_swap(hdr->nob1, 2);

  return pdu;
}

bool
PrimeFrame::deserializeREG(struct prime13_reg_header *hdr, size_t size)
{
  size_t expectedSize;
  this->PDU.genType = GenericType::REG;

  if (size < sizeof(struct prime13_reg_header)) {
    fprintf(stderr,
        "REG: Size is too small (%ld<%ld)\n",
        size,
        sizeof(struct prime13_reg_header));
    return false;
  }

  prime13_nob_swap(hdr->nob1, 2);

  expectedSize = sizeof(struct prime13_reg_header) + 32 * (hdr->spc > 0);

  if (size < expectedSize) {
    fprintf(stderr,
            "REG: Size is smaller than expectedSize (%ld<%ld)\n",
            size,
            expectedSize);
    return false;
  }

  this->PDU.REG.N =       hdr->n;
  this->PDU.REG.R =       hdr->r;
  this->PDU.REG.CAP_SW =  hdr->cap_sw;
  this->PDU.REG.CAP_PA =  hdr->cap_pa;
  this->PDU.REG.CAP_CFP = hdr->cap_cfp;
  this->PDU.REG.CAP_DC =  hdr->cap_dc;
  this->PDU.REG.CAP_MC =  hdr->cap_mc;
  this->PDU.REG.CAP_ARQ = hdr->cap_arq;
  this->PDU.REG.SPC =     hdr->spc;
  this->PDU.REG.TIME =    hdr->time;

  memcpy(this->PDU.REG.EUI_48, hdr->eui48, 6);

  if (this->PDU.REG.SPC > 0) {
    memcpy(this->PDU.REG.SNK, hdr->keys[0].snk, 16);
    memcpy(this->PDU.REG.AUK, hdr->keys[0].auk, 16);
  }

  return true;
}

std::vector<uint8_t>
PrimeFrame::serializePRO(void) const
{
  size_t expectedSize;
  std::vector<uint8_t> pdu;
  bool is_pro_req_s =
      !this->PDU.HDR.DO && !this->PDU.PRO.N && this->PDU.PRO.NSID == 0xff;
  struct prime13_pro_header *hdr;

  expectedSize = is_pro_req_s ? sizeof(struct prime13_pro_header) : 2;

  pdu.resize(expectedSize);

  hdr = reinterpret_cast<struct prime13_pro_header *>(pdu.data());

  hdr->n = this->PDU.PRO.N;
  hdr->r = 0;
  hdr->rq = this->PDU.PRO.RQ;
  hdr->time = this->PDU.PRO.TIME;
  hdr->nsid = this->PDU.PRO.TIME;

  if (is_pro_req_s) {
    memcpy(hdr->pna, this->PDU.PRO.PNA, 6);
    hdr->swc_dc  = this->PDU.PRO.SWC_DC;
    hdr->swc_mc  = this->PDU.PRO.SWC_MC;
    hdr->swc_arq = this->PDU.PRO.SWC_ARQ;
    hdr->swc_prm = this->PDU.PRO.SWC_PRM;
  }

  return pdu;
}

bool
PrimeFrame::deserializePRO(
    struct prime13_pro_header *hdr,
    size_t size)
{
  bool is_pro_req_s;
  size_t expectedSize;

  if (size < 2) {
    fprintf(stderr, "PRO: Size is too small (%ld<2)\n", size);
    return false;
  }

  this->PDU.genType = GenericType::PRO;

  is_pro_req_s = !this->PDU.HDR.DO && !hdr->n && hdr->nsid == 0xff;
  expectedSize = is_pro_req_s ? sizeof(struct prime13_pro_header) : 2;

  if (size != expectedSize) {
    fprintf(stderr,  "PRO: Unexpected size (%ld != %ld)\n", size, expectedSize);
    return false;
  }

  this->PDU.PRO.N    = hdr->n;
  this->PDU.PRO.RQ   = hdr->rq;
  this->PDU.PRO.TIME = hdr->time;
  this->PDU.PRO.NSID = hdr->nsid;

  if (is_pro_req_s) {
    memcpy(this->PDU.PRO.PNA, hdr->pna, 6);
    this->PDU.PRO.SWC_DC  = hdr->swc_dc;
    this->PDU.PRO.SWC_MC  = hdr->swc_mc;
    this->PDU.PRO.SWC_ARQ = hdr->swc_arq;
    this->PDU.PRO.SWC_PRM = hdr->swc_prm;
  }

  return true;
}

std::vector<uint8_t>
PrimeFrame::serializeCON(void) const
{
  size_t expectedSize;
  std::vector<uint8_t> pdu;
  struct prime13_con_header *hdr;

  expectedSize =
      sizeof(struct prime13_con_header)
      + 6 * (this->PDU.CON.E + this->PDU.CON.D)
      + 2
      + this->PDU.CON.DATA.size();

  pdu.resize(expectedSize);

  hdr = reinterpret_cast<struct prime13_con_header *>(pdu.data());

  hdr->lcid = this->PDU.CON.LCID;
  hdr->e    = this->PDU.CON.E;
  hdr->arq  = this->PDU.CON.ARQ;
  hdr->d    = this->PDU.CON.D;
  hdr->n    = this->PDU.CON.N;

  prime13_nob_swap(hdr->nob1, 2);

  hdr->tail[3 * (this->PDU.CON.E + this->PDU.CON.D)].type =
      this->PDU.CON.TYPE;
  hdr->tail[3 * (this->PDU.CON.E + this->PDU.CON.D)].dlen =
      this->PDU.CON.DLEN;

  if (this->PDU.CON.DLEN > 0)
    memcpy(
        &hdr->tail[3 * (this->PDU.CON.E + this->PDU.CON.D) + 1],
        this->PDU.CON.DATA.data(),
        this->PDU.CON.DLEN);

  if (this->PDU.CON.E)
    memcpy(hdr->eui48, this->PDU.CON.EUI_48, 6);

  if (this->PDU.CON.D) {
    hdr->direct[this->PDU.CON.E].dclcid = this->PDU.CON.DCLCID;
    hdr->direct[this->PDU.CON.E].dclnid = this->PDU.CON.DCLNID;
    hdr->direct[this->PDU.CON.E].dcnad  = this->PDU.CON.DCNAD;
    hdr->direct[this->PDU.CON.E].dcsid  = this->PDU.CON.DCSID;
    hdr->direct[this->PDU.CON.E].dssid  = this->PDU.CON.DSSID;

    prime13_nob_swap(hdr->direct[this->PDU.CON.E].nob2, 2);
    prime13_nob_swap(hdr->direct[this->PDU.CON.E].nob3, 2);
  }

  return pdu;
}

bool
PrimeFrame::deserializeCON(struct prime13_con_header *hdr, size_t size)
{
  size_t expectedSize;
  this->PDU.genType = GenericType::CON;

  if (size < sizeof(struct prime13_con_header)) {
    fprintf(stderr,
        "Size is too small (%ld<%ld)\n",
        size,
        sizeof(struct prime13_con_header));
    return false;
  }

  prime13_nob_swap(hdr->nob1, 2);

  expectedSize = sizeof(struct prime13_con_header)
      + 6 * (hdr->d + hdr->e) + 2;

  if (size < expectedSize) {
    fprintf(stderr,
            "Size is smaller than expectedSize (%ld<%ld)\n",
            size,
            expectedSize);
    return false;
  }


  this->PDU.CON.LCID = hdr->lcid;
  this->PDU.CON.E = hdr->e;
  this->PDU.CON.ARQ = hdr->arq;
  this->PDU.CON.D = hdr->d;
  this->PDU.CON.N = hdr->n;

  this->PDU.CON.TYPE =
      hdr->tail[3 * (this->PDU.CON.E + this->PDU.CON.D)].type;
  this->PDU.CON.DLEN =
      hdr->tail[3 * (this->PDU.CON.E + this->PDU.CON.D)].dlen;

  if (size < expectedSize + this->PDU.CON.DLEN) {
    fprintf(stderr,
            "Size is smaller than expectedSize + DATA (%ld<%ld)\n",
            size,
            expectedSize + this->PDU.CON.DLEN);
  }

  if (this->PDU.CON.DLEN > 0) {
      const uint8_t *data = reinterpret_cast<const uint8_t *>(
          &hdr->tail[3 * (this->PDU.CON.E + this->PDU.CON.D) + 1]);
      this->PDU.CON.DATA.assign(data, data + this->PDU.CON.DLEN);
    }

  if (this->PDU.CON.E)
    memcpy(this->PDU.CON.EUI_48, hdr->eui48, 6);

  if (this->PDU.CON.D) {
    prime13_nob_swap(hdr->direct[this->PDU.CON.E].nob2, 2);
    prime13_nob_swap(hdr->direct[this->PDU.CON.E].nob3, 2);

    this->PDU.CON.DCLCID = hdr->direct[this->PDU.CON.E].dclcid;
    this->PDU.CON.DCLNID = hdr->direct[this->PDU.CON.E].dclnid;
    this->PDU.CON.DCNAD  = hdr->direct[this->PDU.CON.E].dcnad;
    this->PDU.CON.DCSID  = hdr->direct[this->PDU.CON.E].dcsid;
    this->PDU.CON.DSSID  = hdr->direct[this->PDU.CON.E].dssid;
  }


  return true;
}

std::vector<uint8_t>
PrimeFrame::serializeDATA(void) const
{
  std::vector<uint8_t> pdu;
  struct prime13_arq_header *hdr;
  struct prime13_sar_hdr *sar;

  unsigned int arqLen = 0;
  unsigned int n = 0;

  // ARQ length: PKTID + WINSIZE (opt) + NACKIDS + ACKID;

  arqLen = 2 + (this->PDU.ARQ.WINSIZE > 0 ? 1 : 0) + this->PDU.ARQ.NACKID.size();

  // Data message contains the following fields:
  //  ARQ header (s)
  //  SAR header
  //  CL header (Convergence layer)
  //  Payload data

  pdu.resize(arqLen
             + 1
             + (this->PDU.SAR.TYPE == FIRST ? 3 : 0)
             + this->PDU.DATA.size());

  hdr = reinterpret_cast<struct prime13_arq_header *>(pdu.data());

  hdr->m = 1;
  hdr->flush = 1;
  hdr->pktid = this->PDU.ARQ.PKTID;

  if (this->PDU.ARQ.WINSIZE > 0) {
    hdr->info[n].m = 1;
    hdr->info[n].flush = 0;
    hdr->info[n].field = this->PDU.ARQ.WINSIZE;
    ++n;
  }

  for (auto p : this->PDU.ARQ.NACKID) {
    hdr->info[n].m = 1;
    hdr->info[n].flush = 1;
    hdr->info[n].field = p;
    ++n;
  }

  hdr->info[n].m = 0;
  hdr->info[n].flush = 0;
  hdr->info[n].field = this->PDU.ARQ.ACKID;
  n = arqLen;

  sar = reinterpret_cast<struct prime13_sar_hdr *>(pdu.data() + n);
  sar->type  = this->PDU.SAR.TYPE;
  sar->field = this->PDU.SAR.FIELD;
  ++n;

  if (this->PDU.SAR.TYPE == FIRST) {
    pdu[n++] = 0x90;
    if (this->PDU.CL.SRC == 0)
      pdu[n++] = this->PDU.HDR.DO ? 1 : 2;
    else
      pdu[n++] = this->PDU.CL.SRC;
    if (this->PDU.CL.DEST == 0)
      pdu[n++] = this->PDU.HDR.DO ? 2 : 1;
    else
      pdu[n++] = this->PDU.CL.DEST;

  }

  std::copy(
      this->PDU.DATA.begin(),
      this->PDU.DATA.end(),
      pdu.begin() + n);

  return pdu;
}

bool
PrimeFrame::deserializeDATA(const uint8_t *payload, size_t size)
{
  unsigned int p = 0;
  const struct prime13_arq_header *arq =
      reinterpret_cast<const struct prime13_arq_header*>(payload);
  const struct prime13_sar_hdr *sar;

  if (size < 1)
    return false;

  this->PDU.genType = GenericType::DATA;

  // Extract ARQ subheader
  this->PDU.ARQ.PKTID = arq->pktid;

  if (arq->m) {
    if (size < 2)
      return false;

    do {
      if (arq->info[p].m && !arq->info[p].flush) { // WINSIZE
        this->PDU.ARQ.WINSIZE = arq->info[p].win.winsize;
      } else if (arq->info[p].m && arq->info[p].flush) { // NACKID
        this->PDU.ARQ.NACKID.push_back(arq->info[p].nack.nackid);
      } else { // ACKID
        this->PDU.ARQ.ACKID = arq->info[p].ack.ackid;
      }
    } while (arq->info[p++].m && p < (size - 1));
  }

  if (p > 0 && arq->info[p - 1].m)
    return false;

  // Consumed bytes: p + 1. Bytes to add: size - p - 1

  payload += p + 1;
  size    -= p + 1;


  // TODO: CHECK THIS!!!!!!!!!!

  if (size < 1)
    return false;

  sar = reinterpret_cast<const struct prime13_sar_hdr *>(payload);

  this->PDU.SAR.TYPE  = static_cast<PrimeFrame::FragmentType>(sar->type);
  this->PDU.SAR.FIELD = sar->field;

  if (sar->type == RESERVED)
    return false;

  ++payload;
  --size;

  if (size < 3)
    return false;

  this->PDU.CL.TYPE = payload[0];
  this->PDU.CL.SRC  = payload[1];
  this->PDU.CL.DEST = payload[2];

  payload += 3;
  size -= 3;

  this->PDU.DATA.assign(payload, payload + size);

  return true;
}

std::vector<uint8_t>
PrimeFrame::serializeGeneric(void) const
{
  size_t expectedSize;
  std::vector<uint8_t> pdu;
  std::vector<uint8_t> payload;
  struct prime13_packet_hdr *header;

  switch (this->PDU.genType) {
    case GenericType::CON:
      payload = this->serializeCON();
      break;

    case GenericType::REG:
      payload = this->serializeREG();
      break;

    case GenericType::DATA:
      payload = this->serializeDATA();
      break;

    case GenericType::PRO:
      payload = this->serializePRO();
      break;

    case GenericType::BSI:
    case GenericType::FRA:
    case GenericType::CFP:
    case GenericType::ALV:
    case GenericType::MUL:
    case GenericType::PRM:
    case GenericType::SEC:
      break;
  }

  // No data, something failed
  if (payload.size() == 0)
    return pdu;

  expectedSize = sizeof(struct prime13_packet_hdr) + payload.size();
  pdu.resize(expectedSize);

  header = reinterpret_cast<struct prime13_packet_hdr *>(pdu.data());

  if (this->PDU.genType == GenericType::DATA)
    header->lcid_ctype = this->PDU.PKT.LCID_CTYPE;
  else
    header->lcid_ctype = this->PDU.genType;

  header->c          = this->PDU.genType != GenericType::DATA;
  header->prio       = this->PDU.PKT.PRIO;
  header->nad        = this->PDU.PKT.NAD;
  header->len        = payload.size();
  header->lnid       = this->PDU.PKT.LNID;
  header->sid        = this->PDU.PKT.SID;

  prime13_nob_swap(header->nob1, 2);
  prime13_nob_swap(header->nob2, 4);

  memcpy(header->payload, payload.data(), payload.size());

  return pdu;
}

bool
PrimeFrame::deserializeGeneric(struct prime13_packet_hdr *header)
{
  bool ok = false;

  this->PDU.macType = GENERIC;

  this->PDU.PKT.LCID_CTYPE = header->lcid_ctype;
  this->PDU.PKT.C          = header->c;
  this->PDU.PKT.PRIO       = header->prio;
  this->PDU.PKT.NAD        = header->nad;
  this->PDU.PKT.LEN        = header->len;
  this->PDU.PKT.LNID       = header->lnid;
  this->PDU.PKT.SID        = header->sid;

  if (header->c) {
    switch (header->lcid_ctype) {
      case GenericType::CON:
        ok = this->deserializeCON(header->con, header->len);
        break;

      case GenericType::REG:
        ok = this->deserializeREG(header->reg, header->len);
        break;

      case GenericType::PRO:
        ok = this->deserializePRO(header->pro, header->len);
        break;

      default:
        this->PDU.genType = static_cast<GenericType>(header->lcid_ctype);
        ok = true;
    }
  } else {
    ok = this->deserializeDATA(header->payload, header->len);
    // if (!ok) {
    //   fprintf(stderr, "DESERIALIZE DATA FAILED!!!\n");
    //   PrimeFrame::hexdump(
    //       reinterpret_cast<uint8_t *>(header),
    //       sizeof(header) + this->PDU.PKT.LEN);
    // }

  }

  return ok;
}

bool
PrimeFrame::deserializeBeacon(struct prime13_beacon_pdu_hdr *header)
{
  this->PDU.macType = BEACON;

  prime13_nob_swap(header->nob, 2);

  memcpy(this->sna, header->sna, 6);
  memcpy(this->PDU.BCN.SNA, header->sna, 6);

  this->PDU.BCN.QLTY   = header->qlty;
  this->PDU.BCN.SID    = header->sid;
  this->PDU.BCN.CFP    = header->cfp;
  this->PDU.BCN.POS    = header->pos;
  this->PDU.BCN.CNT    = header->count;
  this->PDU.BCN.FRQ    = header->freq;
  this->PDU.BCN.SEQ    = header->seq;
  this->PDU.BCN.LEVEL  = header->level;
  this->PDU.BCN.DNCOST = header->downcost;
  this->PDU.BCN.UPCOST = header->upcost;

  return true;
}

std::vector<uint8_t>
PrimeFrame::serializeBeacon(void) const
{
  std::vector<uint8_t> pdu;
  struct prime13_beacon_pdu_hdr *header;

  // Don't touch CRC
  pdu.resize(sizeof(struct prime13_beacon_pdu_hdr) - 4);

  header = reinterpret_cast<struct prime13_beacon_pdu_hdr *>(pdu.data());

  header->qlty = this->PDU.BCN.QLTY;
  header->sid = this->PDU.BCN.SID;
  header->cfp = this->PDU.BCN.CFP;
  header->pos = this->PDU.BCN.POS;
  header->count = this->PDU.BCN.CNT;
  header->freq = this->PDU.BCN.FRQ;
  header->seq = this->PDU.BCN.SEQ;
  header->level = this->PDU.BCN.LEVEL;
  header->downcost = this->PDU.BCN.DNCOST;
  header->upcost = this->PDU.BCN.UPCOST;

  header->ht = 2;

  memcpy(header->sna, this->sna, 6);

  prime13_nob_swap(header->nob, 2);

  return pdu;
}

bool
PrimeFrame::deserialize(const uint8_t *data, size_t size)
{
  std::vector<uint8_t> copy;
  struct prime13_generic_mac_hdr *mac;

  copy.assign(data, data + size);

  if (size < 4)
    return false;

  mac = reinterpret_cast<struct prime13_generic_mac_hdr *>(copy.data());

  this->PDU.HDR.DO    = mac->du;
  this->PDU.HDR.HT    = mac->ht;
  this->PDU.HDR.LEVEL = mac->level;
  this->PDU.HDR.HCS   = mac->hcs;

  switch (this->PDU.HDR.HT) {
    uint32_t crc;
    struct prime13_packet_hdr *pkt;
    struct prime13_beacon_pdu_hdr *beacon;

    case 0: // Generic MAC frame
      pkt = reinterpret_cast<struct prime13_packet_hdr *>(copy.data());

      prime13_nob_swap(pkt->nob1, 2);
      prime13_nob_swap(pkt->nob2, 4);

      if (size !=
          pkt->len + sizeof(struct prime13_packet_hdr) + sizeof(uint32_t))
        return false;

      crc = prime13_crc32b(this->sna, data, size - 4);
      prime13_nob_swap(&crc, 4);

      if (crc != *reinterpret_cast<const uint32_t *> (data + size - 4))
        return false;

      if (!this->deserializeGeneric(pkt))
        return false;

      break;

    case 2: // Beacon
      beacon = reinterpret_cast<struct prime13_beacon_pdu_hdr *>(copy.data());

      if (size != sizeof(struct prime13_beacon_pdu_hdr))
        return false;

      crc = prime13_crc32b(nullptr, data, sizeof(struct prime13_beacon_pdu_hdr) - 4);
      prime13_nob_swap(&crc, 4);

      if (crc != beacon->crc)
        return false;

      if (!this->deserializeBeacon(beacon))
        return false;

      break;
  }

  return true;
}

////////////////////////////// Frame serialization /////////////////////////////

void
PrimeFrame::vprintToString(std::string &dest, const char *fmt, va_list ap)
{
  char *buf = vstrbuild(fmt, ap);

  if (buf == nullptr) {
    std::cerr
        << "Allocation error while printing packet data to string."
        << std::endl;
    abort();
  }

  dest += buf;

  free(buf);
}

void
PrimeFrame::printToString(std::string &dest, const char *fmt, ...)
{
  va_list ap;

  va_start(ap, fmt);

  vprintToString(dest, fmt, ap);

  va_end(ap);
}


std::vector<uint8_t>
PrimeFrame::serialize(void) const
{
  std::vector<uint8_t> pdu;
  struct prime13_generic_mac_hdr *mac;
  const uint8_t *sna = nullptr;
  uint32_t crc;

  switch (this->PDU.macType) {
    case GENERIC:
      pdu = this->serializeGeneric();
      if (pdu.size() == 0)
        return pdu;

      mac = reinterpret_cast<struct prime13_generic_mac_hdr *>(pdu.data());

      mac->du    = this->PDU.HDR.DO;
      mac->ht    = this->PDU.HDR.HT;
      mac->level = this->PDU.HDR.LEVEL;
      mac->hcs   = prime13_crc8b(this->sna, pdu.data(), 2);

      sna = this->sna;
      break;

    case BEACON:
      pdu = this->serializeBeacon();
      if (pdu.size() == 0)
        return pdu;
      break;
  }

  // PLEASE NOTE: pdu already has allocation space to store the MAC header,
  // the only increase in size we need to apply is 32 bits for the CRC

  // Compute CRC and finish
  crc = prime13_crc32b(sna, pdu.data(), pdu.size());
  pdu.resize(pdu.size() + 4);
  prime13_nob_swap(&crc, 4); // Make big endian
  memcpy(pdu.data() + pdu.size() - 4, &crc, 4);

  return pdu;
}

void
PrimeFrame::printCON(std::string &dest) const
{
  printToString(dest, "  frame->PDU.CON.LCID   = 0x%x;\n", this->PDU.CON.LCID);
  printToString(dest, "  frame->PDU.CON.N      = 0x%x;\n", this->PDU.CON.N);
  printToString(dest, "  frame->PDU.CON.TYPE   = 0x%x;\n", this->PDU.CON.TYPE);
  printToString(dest, "  frame->PDU.CON.DLEN   = 0x%x;\n", this->PDU.CON.DLEN);
  printToString(dest, "  frame->PDU.CON.E      = 0x%x;\n", this->PDU.CON.E);

  switch (this->PDU.HDR.DO * 2 + this->PDU.CON.N) {
    case 0:
      printToString(dest, "  // TYPE: CON_REQ_S\n");
      break;

    case 1:
      printToString(dest, "  // TYPE: CON_CLS_S\n");
      break;

    case 2:
      printToString(dest, "  // TYPE: CON_REQ_B\n");
      break;

    case 3:
      printToString(dest, "  // TYPE: CON_CLS_B\n");
      break;
  }

  if (this->PDU.CON.E)
    printToString(dest,
        "  memcpy(frame->PDU.CON.EUI48, hexStrToVec(\"%02x%02x%02x%02x%02x%02x\").data(), 6);\n",
        this->PDU.CON.EUI_48[0],
        this->PDU.CON.EUI_48[1],
        this->PDU.CON.EUI_48[2],
        this->PDU.CON.EUI_48[3],
        this->PDU.CON.EUI_48[4],
        this->PDU.CON.EUI_48[5]);

  printToString(dest, "  frame->PDU.CON.ARQ    = 0x%x;\n", this->PDU.CON.ARQ);
  printToString(dest, "  frame->PDU.CON.D      = 0x%x;\n",    this->PDU.CON.D);

  if (this->PDU.CON.D) {
    printToString(dest, "  frame->PDU.CON.DCLCID   = 0x%x;\n", this->PDU.CON.DCLCID);
    printToString(dest, "  frame->PDU.CON.DCLNID   = 0x%x;\n", this->PDU.CON.DCLNID);
    printToString(dest, "  frame->PDU.CON.DCNAD    = 0x%x;\n", this->PDU.CON.DCNAD);
    printToString(dest, "  frame->PDU.CON.DCSID    = 0x%x;\n", this->PDU.CON.DCSID);
    printToString(dest, "  frame->PDU.CON.DSSID    = 0x%x;\n", this->PDU.CON.DSSID);
  }

  printToString(dest, "  frame->PDU.DATA        = hexStrToVector(\"");
  for (auto p : this->PDU.DATA)
    printToString(dest, "%02x", p);
  printToString(dest, "\");\n");

  PrimeFrame::hexdump(
        dest,
        this->PDU.DATA.data(),
        this->PDU.DATA.size());
}

void
PrimeFrame::printREG(std::string &dest) const
{
  printToString(dest, "  frame->PDU.REG.N       = 0x%x;\n", this->PDU.REG.N);
  printToString(dest, "  frame->PDU.REG.R       = 0x%x;\n", this->PDU.REG.R);
  printToString(dest, "  frame->PDU.REG.CAP_SW  = 0x%x;\n", this->PDU.REG.CAP_SW);
  printToString(dest, "  frame->PDU.REG.CAP_PA  = 0x%x;\n", this->PDU.REG.CAP_PA);
  printToString(dest, "  frame->PDU.REG.CAP_CFP = 0x%x;\n", this->PDU.REG.CAP_CFP);
  printToString(dest, "  frame->PDU.REG.CAP_DC  = 0x%x;\n", this->PDU.REG.CAP_DC);
  printToString(dest, "  frame->PDU.REG.CAP_MC  = 0x%x;\n", this->PDU.REG.CAP_MC);
  printToString(dest, "  frame->PDU.REG.CAP_ARQ = 0x%x;\n", this->PDU.REG.CAP_ARQ);
  printToString(dest, "  frame->PDU.REG.SPC     = 0x%x;\n", this->PDU.REG.SPC);
  printToString(dest, "  frame->PDU.REG.TIME    = 0x%x;\n", this->PDU.REG.TIME);

  printToString(dest,
      "  memcpy(frame->PDU.REG.EUI48, hexStrToVec(\"%02x%02x%02x%02x%02x%02x\").data(), 6);\n",
      this->PDU.REG.EUI_48[0],
      this->PDU.REG.EUI_48[1],
      this->PDU.REG.EUI_48[2],
      this->PDU.REG.EUI_48[3],
      this->PDU.REG.EUI_48[4],
      this->PDU.REG.EUI_48[5]);

  if (this->PDU.REG.SPC > 0) {
    printToString(dest,
          "  memcpy(frame->PDU.REG.SNK, hexStrToVec(\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\").data(), 6);\n",
          this->PDU.REG.SNK[0],
          this->PDU.REG.SNK[1],
          this->PDU.REG.SNK[2],
          this->PDU.REG.SNK[3],
          this->PDU.REG.SNK[4],
          this->PDU.REG.SNK[5],
          this->PDU.REG.SNK[6],
          this->PDU.REG.SNK[7],
          this->PDU.REG.SNK[8],
          this->PDU.REG.SNK[9],
          this->PDU.REG.SNK[10],
          this->PDU.REG.SNK[11],
          this->PDU.REG.SNK[12],
          this->PDU.REG.SNK[13],
          this->PDU.REG.SNK[14],
          this->PDU.REG.SNK[15]);
    printToString(dest,
        "  memcpy(frame->PDU.REG.AUK, hexStrToVec(\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\").data(), 6);\n",
        this->PDU.REG.AUK[0],
        this->PDU.REG.AUK[1],
        this->PDU.REG.AUK[2],
        this->PDU.REG.AUK[3],
        this->PDU.REG.AUK[4],
        this->PDU.REG.AUK[5],
        this->PDU.REG.AUK[6],
        this->PDU.REG.AUK[7],
        this->PDU.REG.AUK[8],
        this->PDU.REG.AUK[9],
        this->PDU.REG.AUK[10],
        this->PDU.REG.AUK[11],
        this->PDU.REG.AUK[12],
        this->PDU.REG.AUK[13],
        this->PDU.REG.AUK[14],
        this->PDU.REG.AUK[15]);
  }
}

void
PrimeFrame::printPRO(std::string &dest) const
{
  printToString(dest, "  frame->PDU.PRO.N       = 0x%x;\n", this->PDU.PRO.N);
  printToString(dest, "  frame->PDU.PRO.RQ      = 0x%x;\n", this->PDU.PRO.RQ);
  printToString(dest, "  frame->PDU.PRO.TIME    = 0x%x;\n", this->PDU.PRO.TIME);
  printToString(dest, "  frame->PDU.PRO.NSID    = 0x%x;\n", this->PDU.PRO.NSID);

  if (!this->PDU.HDR.DO && !this->PDU.PRO.N && this->PDU.PRO.NSID == 0xff) {
    printToString(dest,
        "  memcpy(frame->PDU.PRO.PNA, hexStrToVec(\"%02x%02x%02x%02x%02x%02x\").data(), 6);\n",
        this->PDU.PRO.PNA[0],
        this->PDU.PRO.PNA[1],
        this->PDU.PRO.PNA[2],
        this->PDU.PRO.PNA[3],
        this->PDU.PRO.PNA[4],
        this->PDU.PRO.PNA[5]);

    printToString(dest, "  frame->PDU.PRO.SWC_DC  = 0x%x;\n", this->PDU.PRO.SWC_DC);
    printToString(dest, "  frame->PDU.PRO.SWC_MC  = 0x%x;\n", this->PDU.PRO.SWC_MC);
    printToString(dest, "  frame->PDU.PRO.SWC_PRM = 0x%x;\n", this->PDU.PRO.SWC_PRM);
    printToString(dest, "  frame->PDU.PRO.SWC_ARQ = 0x%x;\n", this->PDU.PRO.SWC_ARQ);
  }
}

void
PrimeFrame::printDATA(std::string &dest) const
{
  printToString(dest, "  frame->PDU.ARQ.PKTID   = %d;\n", this->PDU.ARQ.PKTID);
  printToString(dest, "  frame->PDU.ARQ.WINSIZE = 0x%x;\n", this->PDU.ARQ.WINSIZE);
  printToString(dest, "  frame->PDU.ARQ.ACKID   = %d;\n", this->PDU.ARQ.ACKID);

  printToString(dest, "  frame->PDU.ARQ.NACKID.resize(%ld);\n", this->PDU.ARQ.NACKID.size());
  for (auto p : this->PDU.ARQ.NACKID)
    printToString(dest, "  frame->PDU.ARQ.NACKID.push_back(0x%x);\n", p);

  switch (this->PDU.SAR.TYPE) {
    case PrimeFrame::FIRST:
      printToString(dest, "  frame->PDU.SAR.TYPE    = 0; // FIRST\n");
      printToString(dest, "  frame->PDU.SAR.NSEGS   = 0x%x;\n", this->PDU.SAR.NSEGS);
      break;

    case PrimeFrame::INTERMEDIATE:
      printToString(dest, "  frame->PDU.SAR.TYPE    = 1; // INTERMEDIATE\n");
      printToString(dest, "  frame->PDU.SAR.NSEGS   = 0x%x;\n", this->PDU.SAR.SEQ);
      break;

    case PrimeFrame::LAST:
      printToString(dest, "  frame->PDU.SAR.TYPE    = 2; // LAST\n");
      break;

    case PrimeFrame::RESERVED:
      printToString(dest, "  frame->PDU.SAR.TYPE    = 3; // RESERVED?\n");
      break;
  }

  printToString(dest, "\n  frame->PDU.CL.TYPE     = 0x%x;\n", this->PDU.CL.TYPE);
  printToString(dest, "  frame->PDU.CL.SRC      = %d; // %s\n", this->PDU.CL.SRC, this->PDU.CL.SRC == 1 ? "DC" : "METER");
  printToString(dest, "  frame->PDU.CL.DEST     = %d; // %s;\n\n", this->PDU.CL.DEST, this->PDU.CL.DEST == 1 ? "DC" : "METER");
  printToString(dest, "  frame->PDU.DATA        = hexStrToVector(\"");
  for (auto p : this->PDU.DATA)
    printToString(dest, "%02x", p);
  printToString(dest, "\");\n");
  PrimeFrame::hexdump(dest, this->PDU.DATA.data(), this->PDU.DATA.size());
}

void
PrimeFrame::printGeneric(std::string &dest) const
{
  switch (this->PDU.genType) {
    case PrimeFrame::CON:
      printToString(dest, "  frame->PDU.genType     = PrimeFrame::CON;\n");
      break;

    case PrimeFrame::DATA:
      printToString(dest, "  frame->PDU.genType     = PrimeFrame::DATA;\n");
      break;

    case PrimeFrame::REG:
      printToString(dest, "  frame->PDU.genType     = PrimeFrame::REG;\n");
      break;

    case PrimeFrame::PRO:
      printToString(dest, "  frame->PDU.genType     = PrimeFrame::PRO;\n");
      break;

    case GenericType::BSI:
    case GenericType::FRA:
    case GenericType::CFP:
    case GenericType::ALV:
    case GenericType::MUL:
    case GenericType::PRM:
    case GenericType::SEC:
      break;
  }

  printToString(dest, "  frame->PDU.PKT.C       = 0x%x;\n", this->PDU.PKT.C);
  printToString(dest, "  frame->PDU.PKT.PRIO    = 0x%x;\n", this->PDU.PKT.PRIO);
  printToString(dest, "  frame->PDU.PKT.LCID    = 0x%x;\n", this->PDU.PKT.LCID_CTYPE);
  printToString(dest, "  frame->PDU.PKT.NAD     = 0x%x;\n", this->PDU.PKT.NAD);
  printToString(dest, "  frame->PDU.PKT.LEN     = 0x%x;\n", this->PDU.PKT.LEN);
  printToString(dest, "  frame->PDU.PKT.LNID    = 0x%x;\n", this->PDU.PKT.LNID);
  printToString(dest, "  frame->PDU.PKT.SID     = 0x%x;\n", this->PDU.PKT.SID);
  printToString(dest, "  // frame->PDU.PKT.NID  = 0x%x;\n\n", (this->PDU.PKT.SID << 14) | this->PDU.PKT.LNID);

  switch (this->PDU.genType) {
    case PrimeFrame::CON:
      this->printCON(dest);
      break;

    case PrimeFrame::DATA:
      this->printDATA(dest);
      break;

    case PrimeFrame::REG:
      this->printREG(dest);
      break;

    case GenericType::PRO:
      this->printPRO(dest);
      break;

    case GenericType::BSI:
    case GenericType::FRA:
    case GenericType::CFP:
    case GenericType::ALV:
    case GenericType::MUL:
    case GenericType::PRM:
    case GenericType::SEC:
      break;
  }
}

void
PrimeFrame::printBeacon(std::string &dest) const
{
  printToString(dest, "  frame->PDU.BCN.SID    = 0x%x;\n", this->PDU.BCN.SID);
  printToString(dest, "  frame->PDU.BCN.QLTY   = 0x%x;\n", this->PDU.BCN.QLTY);
  printToString(dest, "  frame->PDU.BCN.CFP    = 0x%x;\n", this->PDU.BCN.CFP);
  printToString(dest, "  frame->PDU.BCN.POS    = 0x%x;\n", this->PDU.BCN.POS);
  printToString(dest, "  frame->PDU.BCN.CNT    = 0x%x;\n", this->PDU.BCN.CNT);
  printToString(dest, "  frame->PDU.BCN.FRQ    = 0x%x;\n", this->PDU.BCN.FRQ);
  printToString(dest, "  frame->PDU.BCN.SEQ    = 0x%x;\n", this->PDU.BCN.SEQ);
  printToString(dest, "  frame->PDU.BCN.LEVEL  = 0x%x;\n", this->PDU.BCN.LEVEL);
  printToString(dest, "  frame->PDU.BCN.UPCOST = 0x%x;\n", this->PDU.BCN.UPCOST);
  printToString(dest, "  frame->PDU.BCN.DNCOST = 0x%x;\n", this->PDU.BCN.DNCOST);
}

std::string
PrimeFrame::toString(void) const
{
  std::string dest;

  printToString(dest,
      "  frame = new PrimeFrame(\"%02x:%02x:%02x:%02x:%02x:%02x\");\n",
      this->sna[0],
      this->sna[1],
      this->sna[2],
      this->sna[3],
      this->sna[4],
      this->sna[5]);


  switch (this->PDU.macType) {
    case BEACON:
      printToString(dest, "  frame->PDU.macType     = PrimeFrame::BEACON;\n");
      break;

    case GENERIC:
      printToString(dest, "  frame->PDU.macType     = PrimeFrame::GENERIC;\n");
      break;
  }
  printToString(dest, "  frame->PDU.HDR.HT      = 0x%x;\n", this->PDU.HDR.HT);
  printToString(dest,
      "  frame->PDU.HDR.DO      = 0x%x; // %s\n",
      this->PDU.HDR.DO,
      this->PDU.HDR.DO ? "Downlink: from DC to Meter" : "Uplink: from Meter to DC");
  printToString(dest, "  frame->PDU.HDR.LEVEL   = 0x%x;\n", this->PDU.HDR.LEVEL);
  printToString(dest, "  frame->PDU.HDR.HCS     = 0x%x;\n\n", this->PDU.HDR.HCS);


  switch (this->PDU.macType) {
    case BEACON:
      this->printBeacon(dest);
      break;

    case GENERIC:
      this->printGeneric(dest);
      break;
  }

  return dest;
}

void
PrimeFrame::print(FILE *ofp) const
{
  std::string str =  this->toString();
  fwrite(str.c_str(), str.size(), 1, ofp);
}

///////////////////////////////// Constructor //////////////////////////////////
PrimeFrame::PrimeFrame(const uint8_t *sna)
{
  memset(&this->PDU, 0, sizeof (this->PDU));
  memcpy(this->sna, sna, 6);

  this->PDU.macType = BEACON;
}

PrimeFrame::PrimeFrame(const uint8_t *sna, enum prime13_ctype type) :
  PrimeFrame(sna)
{
  this->PDU.macType = GENERIC;
  this->PDU.genType = static_cast<enum GenericType>(type);
}

PrimeFrame::PrimeFrame(const uint8_t *sna, const uint8_t *data, size_t size) :
  PrimeFrame(sna)
{
  this->PDU.macType = GENERIC;
  this->PDU.genType = GenericType::DATA;
  this->PDU.DATA.assign(data, data + size);
}

PrimeFrame::PrimeFrame(const uint8_t *sna, std::vector<uint8_t> const &data) :
  PrimeFrame(sna)
{
  this->PDU.macType  = GENERIC;
  this->PDU.genType  = GenericType::DATA;
  this->PDU.DATA = data;
}

PrimeFrame *
PrimeFrame::fromRawData(
    const uint8_t *mac,
    const uint8_t *data,
    size_t size)
{
  PrimeFrame *frame = new PrimeFrame(mac);

  if (!frame->deserialize(data, size)) {
    delete frame;
    frame = nullptr;
  }

  return frame;
}

PrimeFrame *
PrimeFrame::fromRawData(const uint8_t *mac, std::vector<uint8_t> const &data)
{
  return PrimeFrame::fromRawData(
      mac,
      data.data(),
      data.size());
}

bool
PrimeFrame::parseSNA(uint8_t *mac, std::string const &string)
{
  unsigned int entries[6];

  if (sscanf(
      string.c_str(),
      "%02x:%02x:%02x:%02x:%02x:%02x",
      entries + 0,
      entries + 1,
      entries + 2,
      entries + 3,
      entries + 4,
      entries + 5) != 6)
    return false;

  mac[0] = entries[0];
  mac[1] = entries[1];
  mac[2] = entries[2];
  mac[3] = entries[3];
  mac[4] = entries[4];
  mac[5] = entries[5];

  return true;
}

bool
PrimeFrame::parseNID(NodeId &id, std::string const &string)
{
  return sscanf(string.c_str(), "%lx", &id) == 1;
}

bool
PrimeFrame::parseLCID(ConnId &id, std::string const &string)
{
  return sscanf(string.c_str(), "%lx", &id) == 1;
}

bool
PrimeFrame::parseInteger(int &id, std::string const &string)
{
  return sscanf(string.c_str(), "%d", &id) == 1;
}

void
PrimeFrame::hexdump(std::string &dest, const void *data, size_t size)
{
  const uint8_t *bytes = (const uint8_t *) data;

  size_t i, j;

  for (i = 0; i < size; ++i) {
    if ((i & 0xf) == 0)
      printToString(dest, "//  %08lx  ", i);

    printToString(dest, "%s%02x ", (i & 0xf) == 8 ? " " : "", bytes[i]);

    if ((i & 0xf) == 0xf) {
      printToString(dest, " | ");

      for (j = i - 15; j <= i; ++j)
        dest += isprint (bytes[j]) ? bytes[j] : '.';

      dest += "\n";
    }
  }

  if ((i & 0xf) != 0) {
    for (j = i; j < __ALIGN (size, 16); ++j)
      printToString(dest, "   %s", (j & 0xf) == 8 ? " " : "");

    printToString(dest, " | ");

    for (j = i & ~0xf; j < size; ++j)
      dest += isprint (bytes[j]) ? bytes[j] : '.';

    dest += "\n";
  }

  printToString(dest, "//  %08lx  \n\n", i);
}

void
PrimeFrame::hexdump(FILE *fp, const void *data, size_t size)
{
  std::string hex;

  hexdump(hex, data, size);

  fwrite(hex.c_str(), hex.size(), 1, fp);
}

void
PrimeFrame::hexdump(const void *data, size_t size)
{
  PrimeFrame::hexdump(stderr, data, size);
}

std::string
PrimeFrame::typeToString(void) const
{
  if (this->PDU.macType == BEACON) {
    return "BEACON";
  } else if (this->PDU.macType == GENERIC) {
    switch (this->PDU.genType) {
      case DATA:
        return "DATA";

      case CON:
        return "CON";

      case REG:
        return "REG";

      case PRO:
        return "PRO";

      case BSI:
        return "BSI";

      case FRA:
        return "FRA";

      case CFP:
        return "CFP";

      case ALV:
        return "ALV";

      case MUL:
        return "MUL";

      case PRM:
        return "PRM";

      case SEC:
        return "SEC";

      default:
        return "UNK (" + std::to_string(this->PDU.genType) + ")";
    }
  } else {
    return "UNKMAC (" + std::to_string(this->PDU.macType) + ")";
  }
}

