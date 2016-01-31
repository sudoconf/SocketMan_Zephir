#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct list_t list_t;

struct list_t {
    list_t    *prev;
    list_t    *next;
};

void        list_init(list_t *head);
bool        list_is_empty(list_t *head);
void        list_add_head(list_t *head, list_t *node);
void        list_add_tail(list_t *head, list_t *node);
void        list_del(list_t *node);

#define list_entry(ptr, type, node) ((type*)((char*)ptr - offsetof(type, node)))

#define list_for_each(cur, head, node)                              \
    for (cur = list_entry((head)->next, typeof(*cur), node);        \
            &cur->node != (head);                                   \
            cur = list_entry(cur->node.next, typeof(*cur), node))
/*
 * safe for removing node while traversing
 */
#define list_for_each_safe(cur, head, node)                         \
    typeof(cur) __next = NULL;                                      \
for (cur = list_entry((head)->next, typeof(*cur), node),            \
        __next = list_entry(cur->node.next, typeof(*cur), node);    \
        &cur->node != (head);                                       \
        cur = __next,                                               \
        __next = list_entry(__next->node.next, typeof(*cur), node))

#define list_get_first(head, type, node) \
    (list_is_empty((head)) ? NULL : list_entry((head)->next, type, node))
#define list_get_last(head, type, node) \
    (list_is_empty((head)) ? NULL : list_entry((head)->prev, type, node))

#endif
