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

ap_abstract0_t* assign(ap_manager_t* man, ap_abstract0_t* zonotope,
		int assignedToVariable, int dim, double *values) {
	ap_linexpr0_t* expression = create_polyhedral_linexpr0(dim, values);
	ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	assignmentArray[0] = expression;

	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = assignedToVariable;
	return ap_abstract0_assign_linexpr_array(man, false, zonotope, tdim,
			assignmentArray, 1, NULL);

}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t* man = t1p_manager_alloc();

	double values28[8][2] = { { 0, LONG_MAX }, { 1007, 2368 }, { LONG_MIN,
			LONG_MAX }, { 0, LONG_MAX }, { -2214, 7813 }, { 0, LONG_MAX }, {
			LONG_MIN, -1 }, { -2317, 6149 } };

	ap_abstract0_t* zonotope28 = create_zonotope(man, dim, values28);
	printf("zonotope28:\n");
	ap_abstract0_fprint(stdout, man, zonotope28, NULL);

	double values[9] =
			{ -6199, 4102, -8218, 6483, 5400, 4370, 3300, -643, -4345 };

	ap_abstract0_t * zonotope35 = assign(man, zonotope28, 0, dim, values);
	printf("zonotope35:\n");
	ap_abstract0_fprint(stdout, man, zonotope35, NULL);

	double values0[8][2] = { { 0, 0 }, { -6469, -1121 }, { LONG_MIN, LONG_MIN },
			{ LONG_MIN, 0 }, { -2034, 8206 }, { 7806, 8741 }, { 1, LONG_MAX }, {
					4244, 5112 } };

	ap_abstract0_t* zonotope0 = create_zonotope(man, dim, values0);
	printf("zonotope0:\n");
	ap_abstract0_fprint(stdout, man, zonotope0, NULL);

	double values24[8][2] = { { -1, LONG_MAX }, { -288, 9838 },
			{ -7594, -2351 }, { -7998, -6561 }, { LONG_MIN, -1 },
			{ -2619, 7239 }, { LONG_MIN, 1 }, { -288, 9838 } };

	ap_abstract0_t* zonotope24 = create_zonotope(man, dim, values24);
	printf("zonotope24:\n");
	ap_abstract0_fprint(stdout, man, zonotope24, NULL);

	ap_abstract0_t* zonotope36 = ap_abstract0_widening(man, zonotope0,
			zonotope24);

	ap_abstract0_t* meet_result = ap_abstract0_meet(man, false, zonotope36,
			zonotope35);
	printf("meet_result:\n");
	ap_abstract0_fprint(stdout, man, meet_result, NULL);
	return 0;
}

