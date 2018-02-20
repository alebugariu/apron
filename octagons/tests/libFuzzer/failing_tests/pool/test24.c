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

bool create_octagon(oct_t** octagon, ap_manager_t* man, oct_t * top, int dim,
		ap_lincons0_array_t constraints) {
	*octagon = oct_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	oct_t* octagon10;
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(1);

	lincons0.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr0 = create_octogonal_linexpr0(dim, 1, 4, -1, 1, 4813);
	lincons0.p[0].linexpr0 = linexpr0;

	if (create_octagon(&octagon10, man, top, dim, lincons0)) {
		printf("succesfully created octagon 10\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, octagon10);
		ap_lincons0_array_fprint(stdout, &a, NULL);

		oct_t* octagon17;
		ap_lincons0_array_t lincons1 = ap_lincons0_array_make(1);

		lincons1.p[0].constyp = AP_CONS_SUPEQ;
		ap_linexpr0_t * linexpr1 = create_octogonal_linexpr0(dim, 2, 4, 1, -1, LONG_MAX);
		lincons1.p[0].linexpr0 = linexpr1;

		if (create_octagon(&octagon17, man, top, dim, lincons1)) {
			printf("succesfully created octagon 17\n");
			a = oct_to_lincons_array(man, octagon17);
			ap_lincons0_array_fprint(stdout, &a, NULL);

			oct_t* octagon34 = oct_meet(man, false, octagon10, octagon17);
			printf("\nsuccesfully obtained octagon 34\n");
			a = oct_to_lincons_array(man, octagon34);
			ap_lincons0_array_fprint(stdout, &a, NULL);

			oct_t* octagon11;
			ap_lincons0_array_t lincons2 = ap_lincons0_array_make(1);
			lincons2.p[0].constyp = AP_CONS_EQ;
			ap_linexpr0_t * linexpr1 = create_octogonal_linexpr0(dim, 1, 4, -1, 1,
					LONG_MAX);
			lincons2.p[0].linexpr0 = linexpr1;

			if (create_octagon(&octagon11, man, top, dim, lincons1)) {
				printf("succesfully created octagon 11\n");
				a = oct_to_lincons_array(man, octagon11);
				ap_lincons0_array_fprint(stdout, &a, NULL);

				oct_t * join_result = oct_join(man, false, octagon34,
						octagon11);
				oct_t * meet_result = oct_meet(man, false, octagon34,
						join_result);
				printf(
						"octagon34 meet (octagon34 join octagon11) == octagon34: ");
				printf("%d\n", (oct_is_eq(man, meet_result, octagon34)));
				printf(
						"octagon34 meet (octagon34 join octagon11) <= octagon34: ");
				printf("%d\n", (oct_is_leq(man, meet_result, octagon34)));
				printf(
						"octagon34 <= octagon34 meet (octagon34 join octagon11): ");
				printf("%d\n", (oct_is_leq(man, octagon34, meet_result)));
			}
		}
	}
	return 0;
}

