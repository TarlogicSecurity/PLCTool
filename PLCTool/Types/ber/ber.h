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

#ifndef _BER_BER_H
#define _BER_BER_H

#include <PLCTool/gurux/include/enums.h>
#include <PLCTool/util/defs.h>
#include <stdio.h>

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
  /**
   * End of Content.
   */
  BER_CI_EOC = 0x00,
  /**
   * Boolean.
   */
  BER_CI_BOOLEAN = 0x1,
  /**
   * Integer.
   */
  BER_CI_INTEGER = 0x2,
  /**
   * Bit String.
   */
  BER_CI_BIT_STRING = 0x3,
  /**
   * Octet string.
   */
  BER_CI_OCTET_STRING = 0x4,
  /**
   * Null value.
   */
  BER_CI_NULL = 0x5,
  /**
   * Object identifier.
   */
  BER_CI_OBJECT_IDENTIFIER = 0x6,
  /**
   * Object Descriptor.
   */
  BER_CI_OBJECT_DESCRIPTOR = 7,
  /**
   * External
   */
  BER_CI_EXTERNAL = 8,
  /**
   * Real (float).
   */
  BER_CI_REAL = 9,
  /**
   * Enumerated.
   */
  BER_CI_ENUMERATED = 10,
  /**
   * Utf8 String.
   */
  BER_CI_UTF8_STRING = 12,
  /**
   * Numeric string.
   */
  BER_CI_NUMERIC_STRING = 18,
  /**
   * Printable string.
   */
  BER_CI_PRINTABLE_STRING = 19,
  /**
   * Teletex string.
   */
  BER_CI_TELETEX_STRING = 20,
  /**
   * Videotex string.
   */
  BER_CI_VIDEOTEX_STRING = 21,
  /**
   * Ia5 string
   */
  BER_CI_IA5_STRING = 22,
  /**
   * Utc time.
   */
  BER_CI_UTC_TIME = 23,
  /**
   * Generalized time.
   */
  BER_CI_GENERALIZED_TIME = 24,
  /**
   * Graphic string.
   */
  BER_CI_GRAPHIC_STRING = 25,
  /**
   * Visible string.
   */
  BER_CI_VISIBLE_STRING = 26,
  /**
   * General string.
   */
  BER_CI_GENERAL_STRING = 27,
  /**
   * Universal string.
   */
  BER_CI_UNIVERSAL_STRING = 28,
  /**
   * Bmp string.
   */
  BER_CI_BMP_STRING = 30,
  /**
   * Application class.
   */
  BER_CI_APPLICATION = 0x40,
  /**
   * Context class.
   */
  BER_CI_CONTEXT = 0x80,
  /**
   * Private class.
   */
  BER_CI_PRIVATE = 0xc0,
  /**
   * Constructed.
   */
  BER_CI_CONSTRUCTED = 0x20,

  /*
   * These types only appear in COSEM data encoding and alias the A-XDR
   * type flags.
   */
  BER_CI_INT32 = 0x20,
  BER_CI_UINT32 = 0x21,
  BER_CI_INT16 = 0x22,
  BER_CI_UINT16 = BER_CI_INTEGER,
  BER_CI_INT8 = 0x24,
  BER_CI_UINT8 = 0x25,
  BER_CI_INT64 = 0x26,
  BER_CI_UINT64 = 0x27,
  BER_CI_FLOAT32 = BER_CI_REAL,
  BER_CI_FLOAT64 = 0x29,
  BER_CI_ENUM = BER_CI_ENUMERATED,
  BER_CI_STRING = BER_CI_PRINTABLE_STRING,
  BER_CI_STRING_UTF8 = BER_CI_UTF8_STRING,
  BER_CI_BINARY_CODED_DESIMAL = BER_CI_NUMERIC_STRING
} BER_CI;

struct ber_type {
  BOOL private_type;
  BOOL context;
  BOOL application;
  BOOL constructed;
  BOOL array;
  uint8_t orig_id;
  union {
    BER_CI ci;
    uint8_t id;
  };

  union {
    /* Buffer elements */
    struct {
      size_t bytecount;
      union {
        uint8_t *bytebuf;
        char *string;
      };
    };

    struct {
      PTR_LIST(struct ber_type, field);
    };

    BOOL as_boolean;
    uint8_t as_byte;
    uint16_t as_int;
    uint32_t as_long;
    uint64_t as_quad;
    uint8_t as_sbyte;
    int16_t as_sint;
    int32_t as_slong;
    int64_t as_squad;

    float as_float;
    uint8_t scalbuf[8];
  };
};

typedef struct ber_type ber_type_t;

void ber_type_debug(const ber_type_t *self, FILE *fp);

BOOL ber_type_is_buffer(const ber_type_t *self);
void ber_type_destroy(ber_type_t *self);
ber_type_t *ber_type_new(uint8_t id);
ber_type_t *ber_type_new_from_dlms(DLMS_DATA_TYPE type);
BOOL ber_type_set_buffer(ber_type_t *self, const void *buf, size_t size);
BOOL ber_type_push(ber_type_t *self, ber_type_t *entry);
BOOL ber_stream_read_objcount(ber_stream_t *self, size_t *len);
BER_CI dlms_data_type_to_ci(DLMS_DATA_TYPE type);
ber_type_t *ber_type_from_stream(ber_stream_t *stream);
ber_type_t *ber_type_from_dlms_data(ber_stream_t *stream);

void ber_fhexdump(FILE *fp, const void *data, uint32_t size);
void ber_hexdump(const void *data, uint32_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BER_BER_H */
