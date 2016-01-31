#include <ev.h>
#include <errno.h>
#include "lib/include/netutils.h"
#include "lib/include/list.h"
#include "lib/include/connection.h"
#include "lib/include/buffer.h"

static void on_accept(struct ev_loop *loop, ev_io *w, int revents);
static void on_readable(struct ev_loop *loop, ev_io *w, int revents);
static void on_writable(struct ev_loop *loop, ev_io *w, int revents);

list_t connection_list;

int start_chat(char *ip , int port) {
    list_init(&connection_list);
    struct ev_loop *loop = ev_loop_new(0);
    int sock = new_tcp_server("0.0.0.0", 8080);
    ev_io listen;
    ev_io_init(&listen, on_accept, sock, EV_READ);
    ev_io_start(loop, &listen);
    ev_run(loop, 0);
    ev_loop_destroy(loop);
    return 0;
}

void
on_accept(struct ev_loop *loop, ev_io *w, int revents) {
    if ((revents | EV_READ) == 0) {
        return ;
    }
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int sock = accept(w->fd, (struct sockaddr*)&addr, &len);
    if (sock < 0) {
        perror("accept");
        return ;
    }
    set_nonblock(sock);
    char p[128];
    get_address(&addr, p, sizeof(p));
    fprintf(stderr, "accept connection from %s\n", p);

    connection_t *c = connection_new();
    c->fd = sock;
    c->loop = loop;
    c->read_watcher.data = c;
    c->write_watcher.data = c;
    ev_io_init(&c->read_watcher, on_readable, sock, EV_READ);
    ev_io_init(&c->write_watcher, on_writable, sock, EV_WRITE);
    list_add_tail(&connection_list, &c->node);
    buffer_init(&c->rbuf);
    buffer_init(&c->wbuf);
    ev_io_start(loop, &c->read_watcher);
}

void
on_readable(struct ev_loop *loop, ev_io *w, int revents) {
    connection_t    *c;
    buffer_t        *wbuf;
    char            buffer[4096];
    int             n;
    c = (connection_t*)w->data;
    wbuf = &c->wbuf;
    while ((n = read(w->fd, buffer, sizeof(buffer))) > 0) {
        /*fprintf(stderr, "receive %d bytes\n", n);*/
        buffer_write_bytes(wbuf, buffer, n);
        c->recv_bytes += n;
    }
    if (n < 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
        fprintf(stderr, "connection is closed recv: %lu, sent: %lu\n",
                c->recv_bytes, c->sent_bytes);
        connection_destroy(c);
    } else if (n == 0 && wbuf->last > wbuf->pos) {
        /*fprintf(stderr, "start write_watcher\n");*/
        ev_io_start(loop, &c->write_watcher);
    } else if (n == 0) {
        connection_destroy(c);
    }
}

void
on_writable(struct ev_loop *loop, ev_io *w, int revents) {
    connection_t    *c;
    buffer_t        *wbuf;
    int             sent;
    c = (connection_t*) w->data;
    wbuf = &c->wbuf;
    while ((sent = write(w->fd, wbuf->pos, wbuf->last - wbuf->pos)) > 0) {
        /*fprintf(stderr, "need send %lu, %d sent\n", wbuf->last - wbuf->pos, sent);*/
        wbuf->pos += sent;
        c->sent_bytes += sent;
        if (wbuf->pos == wbuf->last) {
            buffer_reset(wbuf);
            break;
        }
    }
    if (wbuf->pos == wbuf->last) {
        ev_io_stop(loop, w);
    }
}
