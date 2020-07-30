/*
 * loop.c: Extract frames from SPIP
 *
 * (c) 2019 Tarlogic Security S.L. - All rights reserved
 *
 * Company confidential. Any unauthorized use, disclosure, reproduction or
 * distribution of this file is strictly prohibited.
 */

#include "spip.h"

BOOL
spip_iface_loop(
  spip_iface_t *iface,
  BOOL (*on_pdu) (spip_iface_t *iface, struct spip_pdu *pdu, void *userdata),
  void *userdata)
{
  struct spip_pdu *pdu = NULL;
  BOOL ok;
  
  while (spip_iface_read(iface, &pdu)) {
    ok = (on_pdu)(iface, pdu, userdata);
    spip_iface_dispose(iface, pdu);
    if (!ok)
      return TRUE;
  }
    
  return FALSE;
}