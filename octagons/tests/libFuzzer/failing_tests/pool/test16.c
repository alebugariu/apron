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

bool create_octagon(ap_abstract0_t** octagon, ap_manager_t* man,
		ap_abstract0_t * top, int dim, ap_lincons0_array_t constraints) {
	*octagon = ap_abstract0_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {

	int dim = 8;

	ap_manager_t * man = oct_manager_alloc();
	ap_abstract0_t * top = ap_abstract0_top(man, dim, 0);
	ap_abstract0_t * bottom = ap_abstract0_bottom(man, dim, 0);

	ap_abstract0_t* octagon6;
	ap_lincons0_array_t lincons6 = ap_lincons0_array_make(1);

	lincons6.p[0].constyp = AP_CONS_SUPEQ;
	ap_linexpr0_t * linexpr6 = create_linexpr0(dim, 0, 5, 1, 0, 9223372036854775807.0);
	lincons6.p[0].linexpr0 = linexpr6;

	create_octagon(&octagon6, man, top, dim, lincons6);
	printf("successfully created octagon 6:\n");
	ap_abstract0_fprint(stdout, man, octagon6, NULL);

	printf("LONG_MAX [as long]= %ld\n", LONG_MAX);
	double long_max = LONG_MAX;
	printf("LONG_MAX [as double]= %f\n", long_max);

	ap_abstract0_t* octagon0;
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(1);

	lincons0.p[0].constyp = AP_CONS_SUPEQ;
	ap_linexpr0_t * linexpr0 = create_linexpr0(dim, 0, 5, -1, -1, 1);
	lincons0.p[0].linexpr0 = linexpr0;

	create_octagon(&octagon0, man, top, dim, lincons0);
	printf("\nsuccessfully created octagon 0:\n");
	ap_abstract0_fprint(stdout, man, octagon0, NULL);

	ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	double values[9] = { 0, 0, 0, 0, 0, 0, 0, 0, -9223372036854775808.0 };
	ap_linexpr0_t* assignment_expression = create_polyhedral_linexpr0(dim,
			values);
	assignmentArray[0] = assignment_expression;
	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = 2;

	ap_abstract0_t* octagon32 = ap_abstract0_assign_linexpr_array(man, false, octagon0, tdim,
			assignmentArray, 1, NULL);
	printf("\nsuccessfully created octagon 32 [assign x2=LONG_MIN, octagon0]:\n");
	ap_abstract0_fprint(stdout, man, octagon32, NULL);

	ap_abstract0_t* octagon36 = ap_abstract0_meet(man, false, octagon6,
			octagon32);
	printf("\nsuccessfully created octagon 36 [octagon6 meet octagon 32]:\n");
	ap_abstract0_fprint(stdout, man, octagon36, NULL);

	printf("is bottom octagon 36? %d\n", ap_abstract0_is_bottom(man, octagon36));
	ap_abstract0_t * meet_result = ap_abstract0_meet(man, false, top,
			octagon36);
	printf("\nmeet result:\n");
	ap_abstract0_fprint(stdout, man, octagon36, NULL);
	printf("\ntop meet octagon == octagon: ");
	printf("%d\n", (ap_abstract0_is_eq(man, meet_result, octagon36)));
	printf("top meet octagon <= octagon: ");
	printf("%d\n", (ap_abstract0_is_leq(man, meet_result, octagon36)));
	printf("octagon <= top meet octagon: ");
	printf("%d\n", (ap_abstract0_is_leq(man, octagon36, meet_result)));
	return 0;

}

