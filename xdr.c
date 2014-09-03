#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>

#include "nfs_proxy.h"
#include "rpc_mountd.h"
#include "xdr.h"

bool_t xdr_fhandle3(XDR * xdrs, fhandle3 * objp)
{
  if (!xdr_bytes
      (xdrs, (char **) &objp->fhandle3_val, (u_int *) & objp->fhandle3_len,
       FHSIZE3))
    return FALSE;
  return TRUE;
}

bool_t xdr_mountstat3(XDR * xdrs, mountstat3 * objp)
{
  if (!xdr_enum(xdrs, (enum_t *) objp))
    return FALSE;
  return TRUE;
}

bool_t xdr_mountres3_ok(XDR * xdrs, mountres3_ok * objp)
{
  if (!xdr_fhandle3(xdrs, &objp->fhandle))
    return FALSE;
  if (!xdr_array
      (xdrs, (char **) (void *) &objp->auth_flavors.auth_flavors_val,
       (u_int *) & objp->auth_flavors.auth_flavors_len, ~0, sizeof(int),
       (xdrproc_t) xdr_int))
    return FALSE;
  return TRUE;
}

bool_t xdr_mountres3(XDR * xdrs, mountres3 * objp)
{
  if (!xdr_mountstat3(xdrs, &objp->fhs_status))
    return FALSE;
  switch (objp->fhs_status) {
  case 0:
    if (!xdr_mountres3_ok(xdrs, &objp->mountres3_u.mountinfo))
      return FALSE;
    break;
  default:
    break;
  }
  return TRUE;
}
