#include "../include/list.h"

void list_init(list_t *head) {
  head->next = head;
  head->prev = head;
}

bool list_is_empty(list_t *head) { return head->next == head; }

void list_add_tail(list_t *head, list_t *node) {
  list_t *prev = head->prev;
  list_t *next = head;
  prev->next = node;
  node->prev = prev;
  next->prev = node;
  node->next = next;
}

void list_add_head(list_t *head, list_t *node) {
  list_t *prev = head;
  list_t *next = head->next;
  prev->next = node;
  node->prev = prev;
  next->prev = node;
  node->next = next;
}

void list_del(list_t *node) {
  list_t *prev = node->prev;
  list_t *next = node->next;
  prev->next = next;
  next->prev = prev;
}
