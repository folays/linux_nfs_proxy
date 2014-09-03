#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <rpc/rpc.h>

#include "nfs_proxy.h"

static SVCXPRT *create_tcp_transport(unsigned int port)
{
  SVCXPRT *xprt;
  struct sockaddr_in sin;
  int on = 1;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
  if (bind(sock, (struct sockaddr *) &sin, sizeof(struct sockaddr))) {
    perror("bind");
    fprintf(stderr, "Couldn't bind to tcp port %d\n", port);
    exit(1);
  }

  xprt = svctcp_create(sock, 0, 0);
  if (!xprt)
    err(1, "svctcp_create");

  return xprt;
}


int main()
{
  SVCXPRT *xprt_nfsd, *xprt_mountd;

  xprt_nfsd = create_tcp_transport(2049);
  xprt_mountd = create_tcp_transport(2048);

  rpc_nfsd_register(xprt_nfsd);
  rpc_mountd_register(xprt_mountd);

  svc_run();

  rpc_mountd_unregister();
  rpc_nfsd_unregister();
}
