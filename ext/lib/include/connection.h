#ifndef CONNECTION_H
#define CONNECTION_H

#include <ev.h>
#include "list.h"
#include "buffer.h"

typedef void (*io_event_pt)(struct ev_loop *, ev_io *, int);
typedef void (*timer_pt)(struct ev_loop *, ev_timer *, int);
typedef struct connection_t connection_t;
typedef struct handler_t handler_t;

struct handler_t {
    io_event_pt       on_readable;
    io_event_pt       on_writable;
    timer_pt          on_timeout;
};

struct connection_t {
    int               fd;
    struct ev_loop    *loop;
    ev_io             read_watcher;
    ev_io             write_watcher;
    ev_timer          timer;
    buffer_t          rbuf;
    buffer_t          wbuf;
    handler_t         handler;
    list_t            node;
    size_t            recv_bytes;
    size_t            sent_bytes;
};

connection_t*   connection_new();
void            connection_destroy(connection_t *c);
#endif
