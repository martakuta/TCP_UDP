#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"

#define BUFFER_SIZE 1000

int main(int argc, char *argv[]) {
  int sock;
  struct addrinfo addr_hints;
  struct addrinfo *addr_result;

  int i, sflags;
  size_t len;
  ssize_t snd_len;
  struct sockaddr_in my_address;
  socklen_t rcva_len;

  if (argc < 4) {
    fatal("Usage: %s host port message ...\n", argv[0]);
  }

  // 'converting' host/port in string to struct addrinfo
  (void) memset(&addr_hints, 0, sizeof(struct addrinfo));
  addr_hints.ai_family = AF_INET; // IPv4
  addr_hints.ai_socktype = SOCK_DGRAM;
  addr_hints.ai_protocol = IPPROTO_UDP;
  addr_hints.ai_flags = 0;
  addr_hints.ai_addrlen = 0;
  addr_hints.ai_addr = NULL;
  addr_hints.ai_canonname = NULL;
  addr_hints.ai_next = NULL;
  if (getaddrinfo(argv[1], NULL, &addr_hints, &addr_result) != 0) {
    syserr("getaddrinfo");
  }

  my_address.sin_family = AF_INET; // IPv4
  my_address.sin_addr.s_addr =
      ((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
  my_address.sin_port = htons((uint16_t) atoi(argv[2])); // port from the command line

  freeaddrinfo(addr_result);

  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    syserr("socket");
  rcva_len = (socklen_t) sizeof(my_address);
  int con = connect(sock, (struct sockaddr *) &my_address, rcva_len);
  if (con < 0)
    syserr("connect");

  int n = atoi(argv[3]);
  int k = atoi(argv[4]);
  if (k > BUFFER_SIZE)
    k = BUFFER_SIZE;

  char msg[k];
  (void) memset(msg, 49, k);
  len = k;
  sflags = 0;

  for (i = 1; i <= n; i++) {

    (void) memset(msg, 49, k);
    len = k;
    sflags = 0;

    snd_len = send(sock, msg, len, sflags);
    if (snd_len != (ssize_t) len) {
      syserr("partial / failed write");
    }

    (void) printf("sent to socket: %d times %d bytes\n", i, k);
  }

  if (close(sock) == -1) { //very rare errors can occur here, but then
    syserr("close"); //it's healthy to do the check
  }

  return 0;
}
