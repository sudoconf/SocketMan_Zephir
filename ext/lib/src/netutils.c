#include "../include/netutils.h"

int new_tcp_server(const char *host, short port) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    set_reuse_sock(sock);
    set_nonblock(sock);
    set_address(host, port, &addr);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        return -1;
    }
    listen(sock, MAX_BACKLOG);

    return sock;
}

int new_tcp_client(const char *svr, short port) {
    int sock;
    int rc;
    struct sockaddr_in addr;

    set_address(svr, port, &addr);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    rc = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        close(sock);
        return -1;
    }
    return sock;
}

int set_nonblock(int fd) {
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int set_reuse_sock(int sock) {
    int ok = 1;
    return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(ok));
}

int set_address(const char *host, short port, struct sockaddr_in *addr) {
    char *name = strdup(host);
    char *p = strchr(host, ':');
    struct hostent *h = NULL;
    //~ parse host:port
    if (p != NULL) {
        *p++ = '\0';
        port = (short) atoi(p);
        if (port == 0) {
            free(name);
            return -1;
        }
    } else if (port == 0) {
        free(name);
        return -1;
    }
    //~ resolve host
    h = gethostbyname(name);
    if (h == NULL || h->h_addrtype != AF_INET) {
        return -1;
    }

    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr = *(struct in_addr *) h->h_addr;
    free(name);
    return 0;
}

char *get_address(struct sockaddr_in *addr, char *p, size_t len) {
    const char *s =
            inet_ntop(AF_INET, (struct sockaddr *) &(addr->sin_addr), p, len);
    if (s != NULL) {
        sprintf(p + strlen(p), ":%d", ntohs(addr->sin_port));
    }
    return (char *) s;
}

int writen(int fd, const char *b, int n) {
    int sent = 0;

    while (n > 0) {
        sent = write(fd, b, n);
        if (sent < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("write");
                return -1;
            } else {
                fprintf(stderr, "slow down\n");
                usleep(100);
            }
        } else {
            n -= sent;
            b += sent;
        }
    }
    return 0;
}

int readn(int fd, char *b, int n) { return 0; }
