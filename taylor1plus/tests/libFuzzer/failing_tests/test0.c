#include <time.h>
#include "t1p.h"
#include <string.h>
#include <stdio.h>

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
	printf("Before assignment!");
	fflush(stdout);
	ap_abstract0_t* result = ap_abstract0_assign_linexpr_array(man, false,
			zonotope, tdim, assignmentArray, 1, NULL);
	ap_abstract0_fprint(stdout, man, result, NULL);
	free(assignmentArray);
	free(tdim);
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t* man = t1p_manager_alloc();

	double values0[8][2] = { { 448, 4819 }, { -6469, -1121 }, { 4831, 9743 }, {
			LONG_MIN, 0 }, { -2034, 8206 }, { 7806, 8741 }, { 0, LONG_MAX }, {
			4244, 5112 } };
	ap_abstract0_t* zonotope0 = create_zonotope(man, dim, values0);
	ap_abstract0_fprint(stdout, man, zonotope0, NULL);

	ap_manager_free(man);
	man = t1p_manager_alloc();

	double assignmentValues0[9] = { 686832606, 1812014285, 845477147, 589228643,
			240314275, LONG_MIN, 489148821, LONG_MIN, 340713449 };
	assign(man, zonotope0, 0, dim, assignmentValues0);

	printf("Finished!\n");
	return 0;
}

