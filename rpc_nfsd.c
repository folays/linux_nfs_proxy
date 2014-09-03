#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <rpc/rpc.h>

#include "nfs_proxy.h"
#include "rpc_nfsd.h"

#define NFSPROC3_NULL 0

void *nfsproc3_null_3_svc(U(void *argp), U(struct svc_req *rqstp))
{
  static void *result = NULL;

  return &result;
}

static void nfs3_program_3(struct svc_req *rqstp, register SVCXPRT * transp)
{
  char *result;
  xdrproc_t _xdr_argument, _xdr_result;
  char *(*local) (char *, struct svc_req *);

  union {
    int unused;
    /* GETATTR3args nfsproc3_getattr_3_arg; */
    /* SETATTR3args nfsproc3_setattr_3_arg; */
    /* LOOKUP3args nfsproc3_lookup_3_arg; */
    /* ACCESS3args nfsproc3_access_3_arg; */
    /* READLINK3args nfsproc3_readlink_3_arg; */
    /* READ3args nfsproc3_read_3_arg; */
    /* WRITE3args nfsproc3_write_3_arg; */
    /* CREATE3args nfsproc3_create_3_arg; */
    /* MKDIR3args nfsproc3_mkdir_3_arg; */
    /* SYMLINK3args nfsproc3_symlink_3_arg; */
    /* MKNOD3args nfsproc3_mknod_3_arg; */
    /* REMOVE3args nfsproc3_remove_3_arg; */
    /* RMDIR3args nfsproc3_rmdir_3_arg; */
    /* RENAME3args nfsproc3_rename_3_arg; */
    /* LINK3args nfsproc3_link_3_arg; */
    /* READDIR3args nfsproc3_readdir_3_arg; */
    /* READDIRPLUS3args nfsproc3_readdirplus_3_arg; */
    /* FSSTAT3args nfsproc3_fsstat_3_arg; */
    /* FSINFO3args nfsproc3_fsinfo_3_arg; */
    /* PATHCONF3args nfsproc3_pathconf_3_arg; */
    /* COMMIT3args nfsproc3_commit_3_arg; */
  } argument;
  printf("%s : rqstp->rq_proc %lu\n", __func__, rqstp->rq_proc);
  switch (rqstp->rq_proc)
    {
    case NFSPROC3_NULL:
      _xdr_argument = (xdrproc_t) xdr_void;
      _xdr_result = (xdrproc_t) xdr_void;
      local = (char *(*)(char *, struct svc_req *)) nfsproc3_null_3_svc;
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
  return;
}

void rpc_nfsd_unregister()
{
  svc_unregister(NFS3_PROGRAM, NFS_V3);
}

void rpc_nfsd_register(SVCXPRT *xprt)
{
  pmap_unset(NFS3_PROGRAM, NFS_V3);
  if (!svc_register(xprt, NFS3_PROGRAM, NFS_V3, nfs3_program_3, IPPROTO_TCP))
    err(1, "svc_register nfsd");
}
