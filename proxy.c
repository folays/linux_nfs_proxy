#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <rpc/rpc.h>
#include <rpc/xdr.h>
#include <rpc/pmap_prot.h>

#include "rpc_nfsd.h"
#include "rpc_mountd.h"
#include "xdr.h"

static const struct timeval TIMEOUT = { 20, 0 };
static const struct timeval RETRY_TIMEOUT = { 3, 0 };
#define MNT_SENDBUFSIZE (2048U)
#define MNT_RECVBUFSIZE (1024U)

typedef struct {
  char **hostname;
  struct sockaddr_in saddr;
  struct pmap pmap;
} clnt_addr_t;

void proxy_mount(int s)
{
  CLIENT *clnt;
  dirpath *mnt3arg;
  mountres3 *mnt3res;

  char *hostname, *mounthost = NULL;

  clnt_addr_t mnt_server = {
    .hostname = &mounthost
  };
  struct pmap *mnt_pmap = &mnt_server.pmap;
  memset(mnt_pmap, '\0', sizeof(*mnt_pmap));
  mnt_pmap->pm_prog = MOUNTPROG;

  clnt_addr_t nfs_server = {
    .hostname = &hostname
  };
  struct pmap *nfs_pmap = &nfs_server.pmap;
  memset(nfs_pmap, '\0', sizeof(*nfs_pmap));
  nfs_pmap->pm_prog = NFS3_PROGRAM;

  mnt_server.saddr.sin_family = AF_INET;
  mnt_server.saddr.sin_port = htons(2049);
  mnt_server.saddr.sin_addr.s_addr = inet_addr("10.42.2.99");

  char *dirname = "/bli";
  mountres3 mntres;

  {
  /* clnt = mnt_openclnt(mnt_server, &msock); */
    clnt = clnttcp_create(&mnt_server.saddr, mnt_pmap->pm_prog, mnt_pmap->pm_vers,
			  &s, MNT_SENDBUFSIZE, MNT_RECVBUFSIZE);

  }
  if (!clnt)
    errx(1, "mnt_openclnt");

  /* return */ clnt_call(clnt, MOUNTPROC_MNT,
			 (xdrproc_t) xdr_dirpath, (caddr_t) mnt3arg,
			 (xdrproc_t) xdr_mountres3, (caddr_t) mnt3res,
			 TIMEOUT);
}

void proxy_()
{
  /* struct sockaddr_in sin; */

  /* sin.sin_family = AF_INET; */
  /* sin.sin_port = 0; */
  /* sin.sin_addr.s_addr = INADDR_ANY; */

  /* int s = socket(AF_INET, SOCK_STREAM, 0); */
  /* if (bind(s, (struct sockaddr *)&sin, sizeof(struct sockaddr))) */
  /*   err(1, "bind"); */

  /* sin.sin_family = AF_INET; */
  /* sin.sin_port = htons(2049); */
  /* sin.sin_addr.s_addr = inet_addr("10.42.2.99"); */

  /* if (connect(s, (struct sockaddr *)&sin, sizeof(sin))) */
  /*   err(1, "connect"); */

  /* proxy_mount(s); */
  proxy_mount(999);
}
