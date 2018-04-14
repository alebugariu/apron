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

	oct_t* aux;

	ap_lincons0_array_t linconsaux = ap_lincons0_array_make(1);

	linconsaux.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * linexpraux = create_linexpr0(dim, 4, 5, 0, 1, -4819);
	linconsaux.p[0].linexpr0 = linexpraux;

	if (create_octagon(&aux, man, top, dim, linconsaux)) {
		printf("successfully created octagon aux:\n");
		ap_lincons0_array_t a = oct_to_lincons_array(man, aux);
		ap_lincons0_array_fprint(stdout, &a, NULL);
		printf("\n");
	}

	oct_t* octagon32 = oct_meet(man, false, octagon29, aux);

	ap_lincons0_array_t a = oct_to_lincons_array(man, octagon32);
	ap_lincons0_array_fprint(stdout, &a, NULL);
	printf("\n");

	/*oct_t* octagon36;

	 ap_lincons0_array_t lincons0 = ap_lincons0_array_make(7);

	 lincons0.p[0].constyp = AP_CONS_EQ;
	 ap_linexpr0_t * linexpr0 = create_linexpr0(dim, 4, 5, 1, 0, 1266);
	 lincons0.p[0].linexpr0 = linexpr0;

	 lincons0.p[1].constyp = AP_CONS_EQ;
	 ap_linexpr0_t * linexpr1 = create_linexpr0(dim, 4, 5, 1, 1, 6085);
	 lincons0.p[1].linexpr0 = linexpr1;

	 lincons0.p[2].constyp = AP_CONS_EQ;
	 ap_linexpr0_t * linexpr2 = create_linexpr0(dim, 4, 5, 0, 1, 4819);
	 lincons0.p[2].linexpr0 = linexpr2;

	 lincons0.p[3].constyp = AP_CONS_EQ;
	 ap_linexpr0_t * linexpr3 = create_linexpr0(dim, 4, 5, 1, -1, -3553);
	 lincons0.p[3].linexpr0 = linexpr3;

	 lincons0.p[4].constyp = AP_CONS_EQ;
	 ap_linexpr0_t * linexpr4 = create_linexpr0(dim, 4, 7, 1, 1,
	 9223372036854779904);
	 lincons0.p[4].linexpr0 = linexpr4;

	 lincons0.p[5].constyp = AP_CONS_EQ;
	 ap_linexpr0_t * linexpr5 = create_linexpr0(dim, 5, 7, 1, 1,
	 9223372036854781952);
	 lincons0.p[5].linexpr0 = linexpr5;

	 lincons0.p[6].constyp = AP_CONS_SUPEQ;
	 ap_linexpr0_t * linexpr6 = create_linexpr0(dim, 5, 1, 0, 1,
	 9223372036854777856);
	 lincons0.p[6].linexpr0 = linexpr6;

	 if (create_octagon(&octagon36, man, top, dim, lincons0)) {
	 printf("successfully created octagon:\n");
	 ap_lincons0_array_t a = oct_to_lincons_array(man, octagon36);
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
	 }*/
	return 0;
}

