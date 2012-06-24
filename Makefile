SOURCES = list.c int_trie.c sparse_mda.c avl_tree.c avl_node.c
OBJECTS = $(SOURCES:.c=.o)
TESTS = $(SOURCES:.c=_test)

CFLAGS = -ansi -pedantic

all: $(OBJECTS) $(TESTS)

list_test: list.o

int_trie_test: int_trie.o

sparse_mda_test: sparse_mda.o int_trie.o

avl_tree_test: avl_tree.o avl_node.o

clean:
	rm -rf $(OBJECTS) $(TESTS) *.dSYM

.PHONY: all
