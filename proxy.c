#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void proxy_()
{
  struct sockaddr_in sin;

  sin.sin_family = AF_INET;
  sin.sin_port = 0;
  sin.sin_addr.s_addr = INADDR_ANY;

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (bind(s, (struct sockaddr *)&sin, sizeof(struct sockaddr)))
    err(1, "bind");

  sin.sin_family = AF_INET;
  sin.sin_port = htons(2049);
  sin.sin_addr.s_addr = inet_addr("10.42.2.99");

  if (connect(s, (struct sockaddr *)&sin, sizeof(sin)))
    err(1, "connect");
}
