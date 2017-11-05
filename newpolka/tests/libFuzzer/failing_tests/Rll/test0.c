#include <time.h>
#include "pk.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

ap_linexpr0_t * create_linexpr0(int dim, long *values) {
	ap_coeff_t *cst, *coeff;
	ap_linexpr0_t * linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, dim);
	cst = &linexpr0->cst;

	ap_scalar_set_double(cst->val.scalar, values[dim]);

	size_t i;
	for (i = 0; i < dim; i++) {
		ap_linterm_t * linterm = &linexpr0->p.linterm[i];
		linterm->dim = i;
		coeff = &linterm->coeff;
		ap_scalar_set_double(cst->val.scalar, values[i]);
	}
	return linexpr0;
}

bool create_polyhedron(pk_t** polyhedron, ap_manager_t* man, pk_t * top,
		int dim, ap_lincons0_array_t constraints) {
	*polyhedron = pk_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {
	int dim = 2;
	long nbcons = 5;

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	pk_t* polyhedron1;

	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(nbcons);
	lincons0.p[0].constyp = AP_CONS_SUPEQ;
	lincons0.p[1].constyp = AP_CONS_EQ;
	lincons0.p[2].constyp = AP_CONS_SUPEQ;
	lincons0.p[3].constyp = AP_CONS_EQ;
	lincons0.p[4].constyp = AP_CONS_EQ;

	long values1[3] = { 131072, 2306405959167180800, 1407374883553280 };
	ap_linexpr0_t * linexpr0 = create_linexpr0(dim, values1);
	lincons0.p[0].linexpr0 = linexpr0;

	long values2[3] = { 0, 0, 0 };
	ap_linexpr0_t * linexpr1 = create_linexpr0(dim, values2);
	lincons0.p[1].linexpr0 = linexpr1;

	long values3[3] = { 1280, 144115188075855872, 0 };
	ap_linexpr0_t * linexpr2 = create_linexpr0(dim, values3);
	lincons0.p[2].linexpr0 = linexpr2;

	long values4[3] = { 35184372088832, 565148976676866, 33554432 };
	ap_linexpr0_t * linexpr3 = create_linexpr0(dim, values4);
	lincons0.p[3].linexpr0 = linexpr3;

	long values5[3] = { 562949953421314, 5497558138880, 0 };
	ap_linexpr0_t * linexpr4 = create_linexpr0(dim, values5);
	lincons0.p[4].linexpr0 = linexpr4;

	if (create_polyhedron(&polyhedron1, man, top, dim, lincons0)) {
		printf("bottom <= polyhedron: ");
		printf("%d\n", pk_is_leq(man, bottom, polyhedron1));
	}
	return 0;
}

