#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>

#include "nfs_proxy.h"
#include "rpc_mountd.h"
#include "xdr.h"

bool_t xdr_dirpath(XDR * xdrs, dirpath * objp)
{
  if (!xdr_string(xdrs, objp, MNTPATHLEN))
    return FALSE;
  return TRUE;
}


mountres3 *mountproc_mnt_3_svc(dirpath *argp, struct svc_req *rqstp)
{
  static mountres3 result;
  printf("%s : argp %s\n", __func__, *argp);

  result.fhs_status = MNT3ERR_NAMETOOLONG;
  return &result;
}

void *mountproc_null_3_svc(U(void *argp), U(struct svc_req *rqstp))
{
  static void *result = NULL;

  return &result;
}

static void mountprog_3(struct svc_req *rqstp, register SVCXPRT * transp)
{
  char *result;
  xdrproc_t _xdr_argument, _xdr_result;
  char *(*local) (char *, struct svc_req *);

  union {
    int unused[128];
    dirpath mountproc_mnt_3_arg;
    /* dirpath mountproc_umnt_3_arg; */
  } argument;

  printf("%s : rqstp->rq_proc %lu\n", __func__, rqstp->rq_proc);
  switch (rqstp->rq_proc) {
  case MOUNTPROC_NULL:
    _xdr_argument = (xdrproc_t) xdr_void;
    _xdr_result = (xdrproc_t) xdr_void;
    local = (char *(*)(char *, struct svc_req *)) mountproc_null_3_svc;
    break;
  case MOUNTPROC_MNT:
    _xdr_argument = (xdrproc_t) xdr_dirpath;
    _xdr_result = (xdrproc_t) xdr_mountres3;
    local = (char *(*)(char *, struct svc_req *)) mountproc_mnt_3_svc;
    break;
  default:
    warnx("%s : noproc", __func__);
    svcerr_noproc(transp);
    return;
  }

  memset((char *) &argument, 0, sizeof(argument));
  if (!svc_getargs(transp, (xdrproc_t) _xdr_argument, (caddr_t) & argument)) {
    svcerr_decode(transp);
    return;
  }
  result = (*local) ((char *) &argument, rqstp);
    if (result != NULL &&
        !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
      svcerr_systemerr(transp);
      warn("unable to send RPC reply");
    }
    if (!svc_freeargs
        (transp, (xdrproc_t) _xdr_argument, (caddr_t) & argument)) {
      warn("unable to free XDR arguments");
    }
}

void rpc_mountd_unregister()
{
  svc_unregister(MOUNTPROG, MOUNTVERS3);
}

void rpc_mountd_register(SVCXPRT *xprt)
{
  pmap_unset(MOUNTPROG, MOUNTVERS3);
  if (!svc_register(xprt, MOUNTPROG, MOUNTVERS3, mountprog_3, IPPROTO_TCP))
    err(1, "svc_register mountd");
}
