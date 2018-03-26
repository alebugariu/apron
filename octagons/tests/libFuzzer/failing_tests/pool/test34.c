#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include <string.h>
#include <stdio.h>

ap_linexpr0_t * create_linexpr0(int dim, long v1, long v2, long coeff1,
		long coeff2, long scalar_value) {
	ap_coeff_t *cst, *coeff;
	ap_linexpr0_t * linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 2);
	cst = &linexpr0->cst;

	ap_scalar_set_double(cst->val.scalar, scalar_value);

	ap_linterm_t * linterm = &linexpr0->p.linterm[0];
	linterm->dim = v1;
	coeff = &linterm->coeff;
	ap_scalar_set_double(coeff->val.scalar, coeff1);

	linterm = &linexpr0->p.linterm[1];
	linterm->dim = v2;
	coeff = &linterm->coeff;
	ap_scalar_set_double(coeff->val.scalar, coeff2);
	return linexpr0;
}

ap_linexpr0_t * create_polyhedral_linexpr0(int dim, long *values) {
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

bool create_zonotope(oct_t** octagon, ap_manager_t* man, oct_t * top, int dim,
		ap_lincons0_array_t constraints) {
	*octagon = oct_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	oct_t* octagon8;
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(1);
	lincons0.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr0 = create_linexpr0(dim, 3, 4, 1, 0, LONG_MAX);
	lincons0.p[0].linexpr0 = linexpr0;

	if (create_zonotope(&octagon8, man, top, dim, lincons0)) {
		printf("succesfully created octagon8:\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, octagon8);
		ap_lincons0_array_fprint(stdout, &a, NULL);

		ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
				sizeof(ap_linexpr0_t*));
		long values[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 768 };
		ap_linexpr0_t* assignment_expression = create_polyhedral_linexpr0(dim,
				values);
		assignmentArray[0] = assignment_expression;
		ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
		tdim[0] = 4;

		oct_t* octagon44 = oct_assign_linexpr_array(man, false, octagon8, tdim,
				assignmentArray, 1, NULL);

		if (oct_is_bottom(man, octagon44) == false) {
			printf("\noctagon 44 is not bottom!\n");
			a = oct_to_lincons_array(man, octagon8);
			ap_lincons0_array_fprint(stdout, &a, NULL);

			long values[9] = { -9999, -9999, -9999, -9999, -9999, -9999, -9999,
					-9999, -9999 };
			assignment_expression = create_polyhedral_linexpr0(dim, values);
			assignmentArray[0] = assignment_expression;
			ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
			tdim[0] = 6;
			oct_t* assignmentResult = oct_assign_linexpr_array(man, false,
					octagon44, tdim, assignmentArray, 1, NULL);
			printf("\nassignment result:\n");
			a = oct_to_lincons_array(man, assignmentResult);
			ap_lincons0_array_fprint(stdout, &a, NULL);
			printf("Assignment result is bottom: %d",
					oct_is_bottom(man, assignmentResult));
		}
	}
	return 0;
}

