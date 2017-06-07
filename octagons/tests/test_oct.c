#include <time.h>
#include <klee/klee.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

ap_linexpr0_t * create_linexpr0(unsigned short int dim, int v1, int v2,
		int coeff1, int coeff2, int scalar_value) {
	ap_coeff_t *cst, *coeff;
	ap_linexpr0_t * linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 2);
	cst = &linexpr0->cst;

	ap_scalar_reinit(cst->val.scalar, AP_SCALAR_DOUBLE);
	cst->val.scalar->val.dbl = (double) scalar_value;

	ap_linterm_t * linterm = &linexpr0->p.linterm[0];
	linterm->dim = v1;
	coeff = &linterm->coeff;
	ap_scalar_reinit(coeff->val.scalar, AP_SCALAR_DOUBLE);
	coeff->val.scalar->val.dbl = (double) coeff1;

	linterm = &linexpr0->p.linterm[1];
	linterm->dim = v2;
	coeff = &linterm->coeff;
	ap_scalar_reinit(coeff->val.scalar, AP_SCALAR_DOUBLE);
	coeff->val.scalar->val.dbl = (double) coeff2;
	return linexpr0;
}

ap_lincons0_array_t create_constraints(unsigned short int dim, char * octagonNumber) {
	size_t i;
	size_t nbcons;

	char buffer_nbcons[80] = "number of constraints for octagon ";
	char buffer_type[80] = "type for octagon ";
	char buffer_sym[80] = "symbolic variables for octagon ";
	char buffer_scalar[80] = "scalar value for octagon ";

	klee_make_symbolic(&nbcons, sizeof(nbcons), strcat(buffer_nbcons, octagonNumber));
	klee_assume(nbcons >= MIN_NBCONS & nbcons <= MAX_NBCONS);
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(nbcons);

	int symbolicValues[nbcons][5];
	klee_make_symbolic(symbolicValues, sizeof(symbolicValues),
			strcat(buffer_sym, octagonNumber));
	for (i = 0; i < nbcons; i++) {
		ap_constyp_t type;
		klee_make_symbolic(&type, sizeof(type),
				strcat(buffer_type, octagonNumber));
		klee_assume(type == AP_CONS_SUPEQ);
		lincons0.p[i].constyp = type;
		klee_assume(
				symbolicValues[i][0] < dim & symbolicValues[i][1] < dim
						& symbolicValues[i][0] != symbolicValues[i][1]
						& symbolicValues[i][0] >= 0
						& symbolicValues[i][1] >= 0);

		klee_assume(symbolicValues[i][2] == 1 | symbolicValues[i][2] == -1 | symbolicValues[i][2] == 0);
		klee_assume(symbolicValues[i][3] == 1 | symbolicValues[i][3] == -1 | symbolicValues[i][3] == 0);
        klee_assume(symbolicValues[i][4] > 0);
		ap_linexpr0_t * linexpr0 = create_linexpr0(dim, symbolicValues[i][0],
				symbolicValues[i][1], symbolicValues[i][2],
				symbolicValues[i][3], symbolicValues[i][4]);
		lincons0.p[i].linexpr0 = linexpr0;
	}
	return lincons0;
}

char * linexpr0_toString(ap_linexpr0_t* a) {
	size_t i;
	ap_scalar_t* pscalar = 0;
	ap_coeff_t* coeff;
	ap_dim_t dim;
	bool first;
	int sgn;

	char * constraintAsString = (char *) calloc(500, sizeof(char));
	char sign[5];
	char value[15];
	char buffer[50];
	strcpy(constraintAsString, "");
	strcpy(sign, "");
	strcpy(value, "");
	strcpy(buffer, "");

	double scalar;

	first = true;
	ap_linexpr0_ForeachLinterm(a,i,dim,coeff)
	{
		switch (coeff->discr) {
		case AP_COEFF_SCALAR:
			pscalar = coeff->val.scalar;
			sgn = ap_scalar_sgn(pscalar);
			if (sgn > 0) {
				scalar = pscalar->val.dbl;
				if (!first) {
					strcpy(sign, " + ");
				}
			} else {
				scalar = -pscalar->val.dbl;
				strcpy(sign, " - ");
			}
			break;
		case AP_COEFF_INTERVAL:
			break;
		}
		strcpy(value, "");
		strcpy(buffer, "");
		snprintf(value, 20, "%.*g", 10, scalar + 0.0);
		snprintf(buffer, 500, "%s%s%s*x%lu", constraintAsString, sign, value,
				(unsigned long) dim);
		strcpy(constraintAsString, buffer);
		first = false;
	}
	/* Constant */
	if (first || !ap_coeff_zero(&a->cst)) {
		switch (a->cst.discr) {
		case AP_COEFF_SCALAR:
			pscalar = a->cst.val.scalar;
			sgn = ap_scalar_sgn(pscalar);
			if (sgn >= 0) {
				scalar = pscalar->val.dbl;
				if (!first) {
					strcpy(sign, " + ");
				}
			} else {
				scalar = -pscalar->val.dbl;
				strcpy(sign, " - ");
			}
			strcpy(value, "");
			strcpy(buffer, "");
			snprintf(value, 20, "%.*g", 10, scalar + 0.0);
			snprintf(buffer, 500, "%s%s%s", constraintAsString, sign, value);
			strcpy(constraintAsString, buffer);
			break;
		case AP_COEFF_INTERVAL:
			break;
		}
	}
	return constraintAsString;
}

void print_constraints(ap_lincons0_array_t* array) {
	int i;
	printf("Created array of constraints of size %lu\n",
			(unsigned long) array->size);
	for (i = 0; i < array->size; i++) {
		printf("%d: %s >=0\n", i, linexpr0_toString((&array->p[i])->linexpr0));
	}
}

oct_t* create_octagon(ap_manager_t* man, oct_t * top,
		char * octagonNumber, unsigned short int dim) {
	ap_lincons0_array_t constraints = create_constraints(dim, octagonNumber);
	oct_t* octagon = oct_meet_lincons_array(man, false, top,
			&constraints);
	printf("Created octagon %s!\n", octagonNumber);
	return octagon;
}

void make_symbolic_dimension(unsigned short int *dim) {
	klee_make_symbolic(dim, sizeof(*dim), "number of variables");
	klee_assume(*dim > MIN_DIM & *dim < MAX_DIM);
}
