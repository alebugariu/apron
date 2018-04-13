#include <time.h>
#include "t1p.h"
#include <ap_global1.h>
#include <string.h>
#include <stdio.h>

#define MIN_DIM 8
#define MAX_DIM 8

#define MIN_VALUE -9999
#define MAX_VALUE 9999

#define NB_PREDEFINED_CONSTANTS 5
#define NB_COEFFICIENTS 3
#define PERCENTAGE_PREDEFINED_CONSTANTS 25

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

//uniformly distributed random number in the range [0, max)
long random_with_max(long max) {
	unsigned long num_bins = (unsigned long) max;
	unsigned long num_rand = (unsigned long) RAND_MAX + 1;
	unsigned long bin_size = num_rand / num_bins, defect = num_rand % num_bins;

	long x;
	do {
		x = random();
	} while (num_rand - defect <= (unsigned long) x);

	return x / bin_size;
}

ap_abstract0_t * create_zonotope(ap_manager_t* man, int dim,
		double values[dim][2]) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_double(intervals[i], values[i][0], values[i][1]);
	}
	ap_abstract0_t * zonotope = ap_abstract0_of_box(man, dim, 0, intervals);
	return zonotope;
}

void initialize_zonotopes(ap_manager_t* man, int dim, ap_abstract0_t * top,
		ap_abstract0_t * bottom, unsigned expectedNumber) {
	pool = (ap_abstract0_t **) malloc(34 * sizeof(ap_abstract0_t *));

	unsigned char index1, index2;
	int counter = 0;

	int totalNumber = 2 * expectedNumber * dim;
	int numberPredefinedConstants = (float) PERCENTAGE_PREDEFINED_CONSTANTS
			/ 100 * totalNumber;
	int numberRandom = totalNumber - numberPredefinedConstants;
	int counterRandom = 0;
	int counterPredefined = 0;

	while (counter < expectedNumber) {
		ap_interval_t** intervals = ap_interval_array_alloc(dim);
		int i;
		for (i = 0; i < dim; i++) {
			unsigned percentage1 = random_with_max(100);
			int index1, index2;
			double lowerBound, upperBound;

			if (percentage1 < PERCENTAGE_PREDEFINED_CONSTANTS
					&& counterPredefined < numberPredefinedConstants) {
				index1 = rand() % NB_PREDEFINED_CONSTANTS;
				lowerBound = constants[index1];
				counterPredefined++;
			} else {
				lowerBound = rand() % (MAX_VALUE + 1 - MIN_VALUE) + MIN_VALUE;
			}
			if (percentage1 < PERCENTAGE_PREDEFINED_CONSTANTS
					&& counterPredefined < numberPredefinedConstants) {
				index2 = rand() % NB_PREDEFINED_CONSTANTS;
				upperBound = constants[index2];
				counterPredefined++;
			} else {
				upperBound = rand() % (MAX_VALUE + 1 - MIN_VALUE) + MIN_VALUE;
			}

			double aux;
			if (lowerBound > upperBound) {
				aux = lowerBound;
				lowerBound = upperBound;
				upperBound = aux;
			}
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
	}
	printf("element %d: ", pool_size);
	ap_abstract0_fprint(stdout, man, top, NULL);
	pool[pool_size++] = top;

	printf("element %d: ", pool_size);
	ap_abstract0_fprint(stdout, man, bottom, NULL);
	pool[pool_size++] = bottom;
	initial_pool_size = pool_size;
}

ap_linexpr0_t * create_polyhedral_linexpr0(int dim, double *values) {
	ap_coeff_t *cst, *coeff;
	ap_linexpr0_t * linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, dim);
	cst = &linexpr0->cst;
	ap_scalar_set_double(cst->val.scalar, values[dim]);

	size_t i;
	for (i = 0; i < dim; i++) {
		ap_linterm_t * linterm = &linexpr0->p.linterm[i];
		linterm->dim = i;
		coeff = &linterm->coeff;
		ap_scalar_set_double(coeff->val.scalar, values[i]);
	}
	return linexpr0;
}

void assign(ap_manager_t* man, ap_abstract0_t* zonotope, int assignedToVariable,
		int dim, double *values) {
	ap_linexpr0_t* expression = create_polyhedral_linexpr0(dim, values);
	ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	assignmentArray[0] = expression;

	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = assignedToVariable;
	printf("Here!!!");
	fflush(stdout);
	ap_abstract0_t* result = ap_abstract0_assign_linexpr_array(man, false,
			zonotope, tdim, assignmentArray, 1, NULL);

	if (!ap_abstract0_is_bottom(man, result)
			&& !ap_abstract0_is_top(man, result)
			&& !ap_abstract0_is_eq(man, result, zonotope)
			&& !exists(man, result)) {
		pool[pool_size++] = result;
	} else {
		ap_abstract0_free(man, result);
	}
	free(assignmentArray);
	free(tdim);
}

void remove_old_elements(ap_manager_t* man) {
	int index;
	for (index = initial_pool_size; index < pool_size; index++) {
		ap_abstract0_free(man, pool[index]);
	}
	pool_size = initial_pool_size;
}

int main(int argc, char **argv) {
	srand(SEED);
	int dim = rand() % (MAX_DIM + 1 - MIN_DIM) + MIN_DIM;

	ap_manager_t* man = t1p_manager_alloc();
	ap_abstract0_t* top = ap_abstract0_top(man, dim, 0);
	ap_abstract0_t* bottom = ap_abstract0_bottom(man, dim, 0);

	initialize_zonotopes(man, dim, top, bottom, 30);
	ap_manager_free(man);
	man = t1p_manager_alloc();
	ap_abstract0_t* zonotope0 = pool[0];
	ap_abstract0_fprint(stdout, man, zonotope0, NULL);

	ap_manager_free(man);
	man = t1p_manager_alloc();

	double assignmentValues0[9] = { 686832606, 1812014285, 845477147, 589228643,
			240314275, LONG_MIN, 489148821, LONG_MIN, 340713449 };
	assign(man, zonotope0, 0, dim, assignmentValues0);

	printf("Finished!\n");
	return 0;
}

