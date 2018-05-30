#include <time.h>
#include "t1p.h"
#include <string.h>
#include <stdio.h>

t1p_t * create_zonotope(ap_manager_t* man, int dim, double values[dim][2]) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_double(intervals[i], values[i][0], values[i][1]);
	}
	t1p_t * zonotope = t1p_of_box(man, dim, 0, intervals);
	return zonotope;
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

t1p_t* assign(ap_manager_t* man, t1p_t* zonotope, int assignedToVariable, int dim, double *values) {
	ap_linexpr0_t* expression = create_polyhedral_linexpr0(dim, values);
	ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	assignmentArray[0] = expression;

	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = assignedToVariable;
	return t1p_assign_linexpr_array(man, false, zonotope, tdim, assignmentArray,
			1, NULL);

}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t* man = t1p_manager_alloc();

	double values0[8][2] = { { -8924,-5373 }, { 4487,8456 }, { 4148,5161 },
			{ 5503,7300 }, { -7022,-5259 }, { -9773,2748 }, { 1906,6349 }, {
					-3587,6037 } };
	t1p_t* zonotope20 = create_zonotope(man, dim, values0);
	printf("successfully created zonotope20:\n");
	t1p_fprint(stdout, man, zonotope20, NULL);

	double values[9] = { LONG_MIN, LONG_MIN, LONG_MIN, LONG_MIN, LONG_MIN, LONG_MIN,
			LONG_MIN, LONG_MIN, LONG_MIN };

	t1p_t* assignment_result = assign(man, zonotope20, 7, dim, values);
	t1p_fprint(stdout, man, assignment_result, NULL);
	return 0;
}
