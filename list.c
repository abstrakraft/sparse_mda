#include <stdlib.h>
#include "list.h"

void
list_init(list_node_t *list)
{
	list->next = NULL;
}

void
list_add(list_node_t *list, list_node_t *new)
{
	new->next = list->next;
	list->next = new;
}

list_node_t *
list_next(list_node_t *list)
{
	return list->next;
}
