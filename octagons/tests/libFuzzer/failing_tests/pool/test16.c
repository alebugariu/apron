#include <time.h>
#include "oct.h"
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

bool create_octagon(ap_abstract0_t** octagon, ap_manager_t* man, ap_abstract0_t * top, int dim,
		ap_lincons0_array_t constraints) {
	*octagon = ap_abstract0_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {

	int dim = 8;

	ap_manager_t * man = oct_manager_alloc();
	ap_abstract0_t * top = ap_abstract0_top(man, dim, 0);
	ap_abstract0_t * bottom = ap_abstract0_bottom(man, dim, 0);

	ap_abstract0_t* octagon22;
	ap_lincons0_array_t lincons22 = ap_lincons0_array_make(1);

	lincons22.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr22 = create_linexpr0(dim, 1, 5, 1, -1, LONG_MIN);
	lincons22.p[0].linexpr0 = linexpr22;

	if (create_octagon(&octagon22, man, top, dim, lincons22)) {
		printf("successfully created octagon 22:\n");
		ap_abstract0_fprint(stdout, man, octagon22, NULL);
	}

	ap_abstract0_t* octagon0;
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(1);

	lincons0.p[0].constyp = AP_CONS_SUPEQ;
	ap_linexpr0_t * linexpr0 = create_linexpr0(dim, 0, 5, -1, -1, 1);
	lincons0.p[0].linexpr0 = linexpr0;

	if (create_octagon(&octagon0, man, top, dim, lincons0)) {
		printf("successfully created octagon 0:\n");
		ap_abstract0_fprint(stdout, man, octagon0, NULL);
	}

	ap_abstract0_t* octagon39 = ap_abstract0_meet(man, false, octagon22, octagon0);
	printf("successfully created octagon 39:\n");
	ap_abstract0_fprint(stdout, man, octagon39, NULL);
	ap_abstract0_t * meet_result = ap_abstract0_meet(man, false, top, octagon39);
	printf("\nmeet result:\n");
	ap_abstract0_fprint(stdout, man, octagon39, NULL);
	printf("\ntop meet octagon == octagon: ");
	printf("%d\n", (ap_abstract0_is_eq(man, meet_result, octagon39)));
	printf("top meet octagon <= octagon: ");
	printf("%d\n", (ap_abstract0_is_leq(man, meet_result, octagon39)));
	printf("octagon <= top meet octagon: ");
	printf("%d\n", (ap_abstract0_is_leq(man, octagon39, meet_result)));
	return 0;

}

