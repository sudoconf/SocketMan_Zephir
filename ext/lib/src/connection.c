#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "../include/connection.h"

connection_t *connection_new() {
  connection_t *c = (connection_t *)malloc(sizeof(connection_t));
  assert(c != NULL);
  c->read_watcher.data = c;
  c->write_watcher.data = c;
  c->recv_bytes = 0;
  c->sent_bytes = 0;
  return c;
}

void connection_destroy(connection_t *c) {
  assert(c != NULL);
  ev_io_stop(c->loop, &c->read_watcher);
  ev_io_stop(c->loop, &c->write_watcher);
  ev_timer_stop(c->loop, &c->timer);
  buffer_destroy(&c->rbuf);
  buffer_destroy(&c->wbuf);
  close(c->fd);
  list_del(&c->node);
  free(c);
}
