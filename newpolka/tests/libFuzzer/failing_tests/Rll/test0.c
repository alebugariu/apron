#include <time.h>
#include "pk.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

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

bool create_polyhedron(pk_t** polyhedron, ap_manager_t* man, pk_t * top,
		int dim, ap_lincons0_array_t constraints) {
	*polyhedron = pk_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	pk_t* polyhedron1;

	ap_lincons0_array_t lincons1 = ap_lincons0_array_make(1);
	lincons1.p[0].constyp = AP_CONS_SUPEQ;
	long values1[9] = { 0, 0, 0, 0, 0, -1, -1, 1, LONG_MAX };
	ap_linexpr0_t * linexpr1 = create_polyhedral_linexpr0(dim, values1);
	lincons1.p[0].linexpr0 = linexpr1;

	if (create_polyhedron(&polyhedron1, man, top, dim, lincons1)) {
		printf("created polyhedron 1\n");
		fflush(stdout);
	}

	pk_t* polyhedron2;

	ap_lincons0_array_t lincons2 = ap_lincons0_array_make(1);
	lincons2.p[0].constyp = AP_CONS_SUPEQ;
	long values2[9] = { 0, 0, 0, 0, -1, -1, -1, -1, LONG_MIN };
	ap_linexpr0_t * linexpr2 = create_polyhedral_linexpr0(dim, values2);
	lincons1.p[0].linexpr0 = linexpr2;

	if (create_polyhedron(&polyhedron2, man, top, dim, lincons1)) {
		printf("created polyhedron 2\n");
		fflush(stdout);
	}
	return 0;
}

