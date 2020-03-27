#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "err.h"

#define BUFFER_SIZE   70000
#define QUEUE_LENGTH     5
#define PORT_NUM     10001

int main(int argc, char *argv[])
{
  int sock, msg_sock;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  socklen_t client_address_len;

  char buffer[BUFFER_SIZE+1];
  ssize_t len;

  sock = socket(PF_INET, SOCK_STREAM, 0); // creating IPv4 TCP socket
  if (sock < 0)
    syserr("socket");
  // after socket() call; we should close(sock) on any execution path;
  // since all execution paths exit immediately, sock would be closed when program terminates

  server_address.sin_family = AF_INET; // IPv4
  server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
  server_address.sin_port = htons(PORT_NUM); // listening on port PORT_NUM

  // bind the socket to a concrete address
  if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
    syserr("bind");

  // switch to listening (passive open)
  if (listen(sock, QUEUE_LENGTH) < 0)
    syserr("listen");

  printf("accepting client connections on port %hu\n", ntohs(server_address.sin_port));

  if (argc == 2) { //when want to save to the file
    char* file_name = *(argv+1);
    for (;;) {
        client_address_len = sizeof(client_address);
        // get client connection from the socket
        msg_sock = accept(sock, (struct sockaddr *) &client_address, &client_address_len);
        if (msg_sock < 0)
          syserr("accept");
        len = read(msg_sock, buffer, sizeof(buffer));
		while (len > 0) {
        if (len < 0)
          syserr("reading from client socket");
        else {
		  printf("read from socket: %zd bytes\n", len);
          FILE *fp;
          if ((fp=fopen(file_name, "a"))==NULL) {
            printf ("Can't open the file %s!\n", file_name);
            exit(1);
          }
          buffer[len] = '\0';
          fprintf (fp, "%s", buffer);
          fclose (fp);
          memset(buffer, 0, BUFFER_SIZE+1);
		}
        len = read(msg_sock, buffer, sizeof(buffer));
	  }
	  printf("ending connection\n");
      if (close(msg_sock) < 0)
        syserr("close");
	}
  } else { //when want to write to the console
    for (;;) {
      client_address_len = sizeof(client_address);
      // get client connection from the socket
      msg_sock = accept(sock, (struct sockaddr *) &client_address, &client_address_len);
      if (msg_sock < 0)
        syserr("accept");
      len = read(msg_sock, buffer, sizeof(buffer));
      while (len > 0) {
        if (len < 0)
          syserr("reading from client socket");
        else {
          printf("read from socket: %zd bytes:\n%.*s\n", len, (int) len, buffer);
        }
        len = read(msg_sock, buffer, sizeof(buffer));
      }
      printf("ending connection\n");
      if (close(msg_sock) < 0)
        syserr("close");
    }
  }
  return 0;
}
