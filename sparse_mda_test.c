#include <stdio.h>
#include <float.h>

#include "sparse_mda.h"

#define DIM_COUNT 4
int size[DIM_COUNT] = {3,3,4,5};
#define DEF_VAL DBL_MAX

#define TEST_COUNT 3
int indices[TEST_COUNT][DIM_COUNT] = {{0,0,0,0},
									  {0,0,0,1},
									  {1,2,3,4}};
double values[TEST_COUNT] = {3.5, 7.6, 9.0};

#define DEF_TEST_COUNT 3
int def_indices[DEF_TEST_COUNT][DIM_COUNT] = {{0,0,0,2},
											  {1,2,3,3},
											  {2,2,2,2}};
int
main(int argc, char *argv[])
{
	int idx;
	char pass;
	sparse_mda_t *mda;
	
	if (!(mda = sparse_mda_init(DIM_COUNT, DEF_VAL))) {
		fprintf(stderr, "sparse_mda_init error\n");
		return -1;
	}

	for (idx = 0; idx < TEST_COUNT; idx++) {
		if (sparse_mda_set(mda, indices[idx], values[idx])) {
			fprintf(stderr, "sparse_mda_set error\n");
		}
	}

	pass = 1;
	for (idx = 0; idx < TEST_COUNT; idx++) {
		if (sparse_mda_get(mda, indices[idx]) != values[idx]) {
			pass = 0;
			printf("value test failed (%d)\n", idx);
		}
	}

	for (idx = 0; idx < DEF_TEST_COUNT; idx++) {
		if (sparse_mda_get(mda, def_indices[idx]) != DEF_VAL) {
			pass = 0;
			printf("default value test failed (%d)\n", idx);
		}
	}

	if (pass) {
		printf("test passed!\n");
	}

	return 0;
}
