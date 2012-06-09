SOURCES = list.c int_trie.c sparse_mda.c
OBJECTS = $(SOURCES:.c=.o)
TESTS = $(SOURCES:.c=_test)

CFLAGS = -ansi -pedantic

all: $(OBJECTS) $(TESTS)

list_test: list.o

int_trie_test: int_trie.o

sparse_mda_test: sparse_mda.o int_trie.o

clean:
	rm -f $(OBJECTS) $(TESTS)

.PHONY: all
