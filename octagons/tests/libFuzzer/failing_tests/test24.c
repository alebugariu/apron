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

bool create_octagon(oct_t** octagon, ap_manager_t* man, oct_t * top, int dim,
		ap_lincons0_array_t constraints) {
	*octagon = oct_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {
	int dim = 4;
	int nbcons1 = 2;
	int nbcons2 = 2;

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);
	oct_t* octagon1;

	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(nbcons1);

	lincons0.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr0 = create_linexpr0(dim, 0, 1, 0, -1,
			576462951326679039);
	lincons0.p[0].linexpr0 = linexpr0;

	lincons0.p[1].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr1 = create_linexpr0(dim, 0, 1, -1, 0, 2199023255551);
	lincons0.p[1].linexpr0 = linexpr1;

	oct_t* octagon2;

	ap_lincons0_array_t lincons1 = ap_lincons0_array_make(nbcons2);

	lincons1.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr2 = create_linexpr0(dim, 0, 1, 0, -1, 2199023255551);
	lincons1.p[0].linexpr0 = linexpr2;

	lincons1.p[1].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr3 = create_linexpr0(dim, 0, 1, -1, 0, 4294967295);
	lincons1.p[1].linexpr0 = linexpr3;

	if (create_octagon(&octagon1, man, top, dim, lincons0)) {
		if (create_octagon(&octagon2, man, top, dim, lincons1)) {
			oct_t * join_result = oct_join(man, false, octagon1, octagon2);
			oct_t * meet_result = oct_meet(man, false, octagon1, join_result);
			printf("octagon1 meet (octagon1 join octagon2) == octagon1: ");
			printf("%d\n", (oct_is_eq(man, meet_result, octagon1)));
			printf("octagon1 meet (octagon1 join octagon2) <= octagon1: ");
			printf("%d\n", (oct_is_leq(man, meet_result, octagon1)));
			printf("octagon1 <= octagon1 meet (octagon1 join octagon2): ");
			printf("%d\n", (oct_is_leq(man, octagon1, meet_result)));
		}
	}
	return 0;
}

