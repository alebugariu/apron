#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include <string.h>
#include <stdio.h>

ap_linexpr0_t * create_linexpr0(int dim, long v1, long v2, double coeff1,
		double coeff2, double scalar_value) {
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

bool create_octagon(oct_t** octagon, ap_manager_t* man, oct_t * top, int dim,
		ap_lincons0_array_t constraints) {
	*octagon = oct_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {

	int dim = 8;
	int nbcons = 7;

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	oct_t* octagon22;
	ap_lincons0_array_t lincons22 = ap_lincons0_array_make(1);

	lincons22.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr22 = create_linexpr0(dim, 1, 5, 1, -1, LONG_MIN);
	lincons22.p[0].linexpr0 = linexpr22;

	if (create_octagon(&octagon22, man, top, dim, lincons22)) {
		printf("successfully created octagon 22:\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, octagon22);
		ap_lincons0_array_fprint(stdout, &a, NULL);
		printf("\n");
	}

	oct_t* octagon29;

	ap_lincons0_array_t lincons29 = ap_lincons0_array_make(1);

	lincons29.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr9 = create_linexpr0(dim, 4, 5, 1, 0, -1266);
	lincons29.p[0].linexpr0 = linexpr9;

	if (create_octagon(&octagon29, man, top, dim, lincons29)) {
		printf("successfully created octagon 29:\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, octagon29);
		ap_lincons0_array_fprint(stdout, &a, NULL);
		printf("\n");
	}

	ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	double values[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 4819 };
	ap_linexpr0_t* assignment_expression = create_polyhedral_linexpr0(dim,
			values);
	assignmentArray[0] = assignment_expression;
	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = 5;

	oct_t* octagon32 = oct_assign_linexpr_array(man, false, octagon29, tdim,
			assignmentArray, 1, NULL);

	printf("successfully created octagon 32:\n");
	ap_lincons0_array_t a = oct_to_lincons_array(man, octagon32);
	ap_lincons0_array_fprint(stdout, &a, NULL);
	printf("\n");

	oct_t* octagon3;
	ap_lincons0_array_t lincons3 = ap_lincons0_array_make(1);

	lincons3.p[0].constyp = AP_CONS_SUPEQ;
	ap_linexpr0_t * linexpr3 = create_linexpr0(dim, 4, 7, -1, 1, -LONG_MAX);
	lincons3.p[0].linexpr0 = linexpr3;

	if (create_octagon(&octagon3, man, top, dim, lincons3)) {
		printf("successfully created octagon 3:\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, octagon3);
		ap_lincons0_array_fprint(stdout, &a, NULL);
		printf("\n");
	}

	oct_t* octagon36 = oct_meet(man, false, octagon3, octagon32);
	printf("successfully created octagon 36:\n");
	a = oct_to_lincons_array(man, octagon36);
	ap_lincons0_array_fprint(stdout, &a, NULL);
	oct_t * meet_result = oct_meet(man, false, top, octagon36);
	printf("\nmeet result:\n");
	a = oct_to_lincons_array(man, meet_result);
	ap_lincons0_array_fprint(stdout, &a, NULL);
	printf("\ntop meet octagon == octagon: ");
	printf("%d\n", (oct_is_eq(man, meet_result, octagon36)));
	printf("top meet octagon <= octagon: ");
	printf("%d\n", (oct_is_leq(man, meet_result, octagon36)));
	printf("octagon <= top meet octagon: ");
	printf("%d\n", (oct_is_leq(man, octagon36, meet_result)));
	return 0;
}

