#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include <string.h>
#include <stdio.h>

ap_linexpr0_t * create_octogonal_linexpr0(int dim, long v1, long v2, long coeff1,
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

	oct_t* octagon32;
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(2);

	lincons0.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr0 = create_octogonal_linexpr0(dim, 1, 5, 0, 1, -3374);
	lincons0.p[0].linexpr0 = linexpr0;

	lincons0.p[1].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr1 = create_octogonal_linexpr0(dim, 1, 5, 1, 0, LONG_MAX);
	lincons0.p[1].linexpr0 = linexpr1;

	if (create_zonotope(&octagon32, man, top, dim, lincons0)) {
		printf("succesfully created octagon\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, octagon32);
		ap_lincons0_array_fprint(stdout, &a, NULL);

		oct_t * meet_result = oct_meet(man, false, octagon32, octagon32);
		printf("\nmeet result:\n");
		a = oct_to_lincons_array(man, meet_result);
		ap_lincons0_array_fprint(stdout, &a, NULL);
		printf("\noctagon meet octagon == octagon: ");
		printf("%d\n", (oct_is_eq(man, meet_result, octagon32)));
		printf("octagon meet octagon <= octagon: ");
		printf("%d\n", (oct_is_leq(man, meet_result, octagon32)));
		printf("octagon <= octagon meet octagon: ");
		printf("%d\n", (oct_is_leq(man, octagon32, meet_result)));
	}
	return 0;
}

