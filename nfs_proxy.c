#include <stdio.h>
#include <rpc/rpc.h>

static SVCXPRT *create_tcp_transport(unsigned int port)
{
  SVCXPRT *xprt;
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  sin.sin_addr.s_addr = INADDR_ANY;
  sock = socket(PF_INET, SOCK_STREAM, 0);
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

transp = svctcp_create(sock, 0, 0);

}

int main()
{
  SVCXPRT *xprt;

  svc_unregister(MOUNTPROG, MOUNTVERS3);
  svc_unregister(NFS3_PROGRAM, NFS_V3);

  xprt = create_tcp_transport(2049);

  if (!svc_register(tcptransp, NFS3_PROGRAM, NFS_V3, nfs3_program_3, IPPROTO_TCP))
    err(1, "svc_register #2");

  if (!svc_register(tcptransp, MOUNTPROG, MOUNTVERS3, mountprog_3, IPPROTO_TCP))
    err(1, "svc_register #2");

  
}
