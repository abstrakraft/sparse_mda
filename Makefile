all: list.o list_test int_trie.o int_trie_test sparse_mda.o sparse_mda_test

list_test: list.o

int_trie_test: int_trie.o

sparse_mda_test: sparse_mda.o int_trie.o

CFLAGS = -ansi -pedantic

clean:
	rm -f *.o list_test int_trie_test sparse_mda_test
