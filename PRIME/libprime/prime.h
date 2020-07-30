/*
 * prime.h: PRIME 1.3 main include file
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#ifndef _PRIME_PRIME_H
#define _PRIME_PRIME_H

#include "layer.h"
#include "state.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

uint32_t prime13_crc32b(const uint8_t *sna, const uint8_t *message, size_t len);
uint8_t prime13_crc8b(const uint8_t *sna, const uint8_t *message, size_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRIME_PRIME_H */

