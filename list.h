#ifndef LIST_H

typedef struct list_node list_node_t;

#define LIST_HEADER \
	list_node_t *next

struct list_node {
	LIST_HEADER;
};

void list_init(list_node_t *);
void list_add(list_node_t *, list_node_t*);
list_node_t *list_next(list_node_t *);

#endif
