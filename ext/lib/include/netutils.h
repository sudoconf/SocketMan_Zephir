#ifndef NET_UTILS_H
#define NET_UTILS_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>


#define MAX_BACKLOG 32

int   new_tcp_server(const char *host, short port);
int   new_tcp_client(const char *srv, short port);
int   set_nonblock(int fd);
int   set_reuse_sock(int sock);
int   set_address(const char *host, short port, struct sockaddr_in *addr);
char* get_address(struct sockaddr_in *addr, char *p, size_t len);
int   writen(int fd, const char *b, int n);
int   readn(int fd, char *b, int n);

#endif
