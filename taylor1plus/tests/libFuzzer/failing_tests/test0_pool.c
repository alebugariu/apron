#include <time.h>
#include "t1p.h"
#include <ap_global1.h>
#include <string.h>
#include <stdio.h>

#define MIN_DIM 8
#define MAX_DIM 8

#define NB_PREDEFINED_CONSTANTS 5
#define NB_COEFFICIENTS 3

#define SEED 1046527

const double constants[NB_PREDEFINED_CONSTANTS] =
		{ LONG_MIN, -1, 0, 1, LONG_MAX };

int pool_size = 0;
int initial_pool_size;
ap_abstract0_t ** pool = NULL;

bool exists(ap_manager_t* man, ap_abstract0_t *element) {
	int i;
	for (i = 0; i < pool_size; i++) {
		if (ap_abstract0_is_eq(man, pool[i], element)) {
			return true;
		}
	}
	return false;
}

void initialize_zonotopes(ap_manager_t* man, int dim, ap_abstract0_t * top,
		ap_abstract0_t * bottom, unsigned expectedNumber) {
	pool = (ap_abstract0_t **) malloc(
			(expectedNumber + 2) * sizeof(ap_abstract0_t *));

	unsigned char index1, index2;
	int counter = 0;

	int totalNumber = 2 * expectedNumber * dim;

	while (counter < expectedNumber) {
		ap_interval_t** intervals = ap_interval_array_alloc(dim);
		int i;
		for (i = 0; i < dim; i++) {
			int index1, index2;
			double lowerBound, upperBound;

			index1 = rand() % NB_PREDEFINED_CONSTANTS;
			lowerBound = constants[index1];
			index2 = rand() % NB_PREDEFINED_CONSTANTS;
			upperBound = constants[index2];
			double aux;
			if (lowerBound > upperBound) {
				aux = lowerBound;
				lowerBound = upperBound;
				upperBound = aux;
			}
			printf("[%f, %f]\n", lowerBound, upperBound);
			ap_interval_set_double(intervals[i], lowerBound, upperBound);
		}
		ap_abstract0_t* element = ap_abstract0_of_box(man, dim, 0, intervals);
		if (!ap_abstract0_is_bottom(man, element)
				&& !ap_abstract0_is_top(man, element)
				&& !exists(man, element)) {
			counter++;
			printf("element %d: ", pool_size);
			ap_abstract0_fprint(stdout, man, element, NULL);
			pool[pool_size++] = element;
		} else {
			ap_abstract0_free(man, element);
		}
		printf("\n");
	}
	printf("element %d: ", pool_size);
	ap_abstract0_fprint(stdout, man, top, NULL);
	pool[pool_size++] = top;

	printf("element %d: ", pool_size);
	ap_abstract0_fprint(stdout, man, bottom, NULL);
	pool[pool_size++] = bottom;
	initial_pool_size = pool_size;
}

int main(int argc, char **argv) {
	srand(SEED);
	int dim = rand() % (MAX_DIM + 1 - MIN_DIM) + MIN_DIM;

	ap_manager_t* man = t1p_manager_alloc();
	ap_abstract0_t* top = ap_abstract0_top(man, dim, 0);
	ap_abstract0_t* bottom = ap_abstract0_bottom(man, dim, 0);

	initialize_zonotopes(man, dim, top, bottom, 3);
	ap_abstract0_t* zonotope1 = pool[1];
	ap_abstract0_fprint(stdout, man, zonotope1, NULL);

	ap_abstract0_t* zonotope2 = pool[2];
	ap_abstract0_fprint(stdout, man, zonotope2, NULL);

	ap_abstract0_t* zonotope5 = ap_abstract0_meet(man, false, zonotope2,
			zonotope1);

	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = 4;

	ap_abstract0_t* zonotope6 = ap_abstract0_forget_array(man, false, zonotope5,
			tdim, 1, false);
	printf("After project:\n");
	ap_abstract0_fprint(stdout, man, zonotope6, NULL);

	ap_abstract0_free(man, zonotope5);
	ap_abstract0_free(man, zonotope6);
	printf("Finished!\n");
	return 0;
}

