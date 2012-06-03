#include <stdlib.h>
#include <stdio.h>
#include "list.h"

typedef struct int_list_node int_list_node_t;
struct int_list_node {
	LIST_HEADER;
	int value;
};

void
print_list(int_list_node_t *list)
{
	int_list_node_t *cursor = list;

	while (cursor) {
		printf("%d ", cursor->value);
		cursor = (int_list_node_t *)list_next((list_node_t *)cursor);
	}
	printf("\n");
}

int
main(int argc, char *argv[])
{
	int idx;
	int_list_node_t *head, *cursor, *prev_cursor;

	if (!(head = malloc(sizeof *head))) {
		fprintf(stderr, "malloc error\n");
		return -1;
	}

	list_init((list_node_t *)head);
	head->value = 0;

	cursor = head;
	for (idx = 1; idx<6; idx++) {
		prev_cursor = cursor;
		if (!(cursor = malloc(sizeof *cursor))) {
			fprintf(stderr, "malloc error\n");
			return -1;
		}

		list_add((list_node_t *)prev_cursor, (list_node_t *)cursor);
		cursor->value = idx*2;
	}

	print_list(head);

	return 0;
}
