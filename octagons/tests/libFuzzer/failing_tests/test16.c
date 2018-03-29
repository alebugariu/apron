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
	int dim = 13;
	int nbcons = 17;

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);
	oct_t* octagon1;

	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(nbcons);
	;

	lincons0.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr0 = create_linexpr0(dim, 4, 1, -1, -1,
			2199023255551);
	lincons0.p[0].linexpr0 = linexpr0;

	lincons0.p[1].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr1 = create_linexpr0(dim, 0, 1, 0, -1, 2199023255551);
	lincons0.p[1].linexpr0 = linexpr1;

	lincons0.p[2].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr2 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[2].linexpr0 = linexpr2;

	lincons0.p[3].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr3 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[3].linexpr0 = linexpr3;

	lincons0.p[4].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr4 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[4].linexpr0 = linexpr4;

	lincons0.p[5].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr5 = create_linexpr0(dim, 2, 1, -1, -1,
			2199023255551);
	lincons0.p[5].linexpr0 = linexpr5;

	lincons0.p[6].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr6 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[6].linexpr0 = linexpr6;

	lincons0.p[7].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr7 = create_linexpr0(dim, 0, 1, -1, -1, 2199023255551);
	lincons0.p[7].linexpr0 = linexpr7;

	lincons0.p[8].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr8 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[8].linexpr0 = linexpr8;

	lincons0.p[9].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr9 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[9].linexpr0 = linexpr9;

	lincons0.p[10].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr10 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[10].linexpr0 = linexpr10;

	lincons0.p[11].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr11 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[11].linexpr0 = linexpr11;

	lincons0.p[12].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr12 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[12].linexpr0 = linexpr12;

	lincons0.p[13].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr13 = create_linexpr0(dim, 2, 1, -1, -1,
			2199023255551);
	lincons0.p[13].linexpr0 = linexpr13;

	lincons0.p[14].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr14 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[14].linexpr0 = linexpr14;

	lincons0.p[15].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr15 = create_linexpr0(dim, 0, 1, -1, -1,
			2199023255551);
	lincons0.p[15].linexpr0 = linexpr15;

	lincons0.p[16].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpr16 = create_linexpr0(dim, 3, 1, -1, 0,
			72066390131015681);
	lincons0.p[16].linexpr0 = linexpr16;

	if (create_octagon(&octagon1, man, top, dim, lincons0)) {
		oct_t * meet_result = oct_meet(man, false, top, octagon1);
		printf("top meet octagon == octagon: ");
		printf("%d\n", (oct_is_eq(man, meet_result, octagon1)));
		printf("top meet octagon <= octagon: ");
		printf("%d\n", (oct_is_leq(man, meet_result, octagon1)));
		printf("octagon <= top meet octagon: ");
		printf("%d\n", (oct_is_leq(man, octagon1, meet_result)));
	}
	return 0;
}

