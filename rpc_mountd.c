#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>

#include "nfs_proxy.h"

#define MOUNTPROG 100005
#define MOUNTVERS3 3

#define MOUNTPROC_NULL 0

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
    int unused;
    /* dirpath mountproc_mnt_3_arg; */
    /* dirpath mountproc_umnt_3_arg; */
  } argument;

  switch (rqstp->rq_proc) {
  case MOUNTPROC_NULL:
    _xdr_argument = (xdrproc_t) xdr_void;
    _xdr_result = (xdrproc_t) xdr_void;
    local = (char *(*)(char *, struct svc_req *)) mountproc_null_3_svc;
    break;
  default:
    svcerr_noproc(transp);
    return;

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
}

void rpc_mountd_unregister()
{
  svc_unregister(MOUNTPROG, MOUNTVERS3);
}

void rpc_mountd_register(SVCXPRT *xprt)
{
  if (!svc_register(xprt, MOUNTPROG, MOUNTVERS3, mountprog_3, IPPROTO_TCP))
    err(1, "svc_register #2");
}
