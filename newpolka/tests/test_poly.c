#include <time.h>
#include <klee/klee.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

ap_linexpr0_t * create_linexpr0(unsigned short int dim, int *values) {
	ap_coeff_t *cst, *coeff;
	ap_linexpr0_t * linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,
			dim);
	cst = &linexpr0->cst;

	ap_scalar_reinit(cst->val.scalar, AP_SCALAR_MPQ);
	mpq_set_si(cst->val.scalar->val.mpq, values[dim], 1);

	size_t i;
	for (i = 0; i < dim; i++) {
		ap_linterm_t * linterm = &linexpr0->p.linterm[i];
		linterm->dim = i;
		coeff = &linterm->coeff;
		ap_scalar_reinit(coeff->val.scalar, AP_SCALAR_MPQ);
		mpq_set_si(coeff->val.scalar->val.mpq, values[i], 1);
	}
	return linexpr0;
}

ap_lincons0_array_t create_constraints(unsigned short int dim,
		char * polyhedronNumber) {
	size_t i, j;
	size_t nbcons;

	char buffer_nbcons[80] = "number of constraints for polyhedron ";
	char buffer_type[80] = "type for polyhedron ";
	char buffer_sym[80] = "symbolic coefficients for polyhedron ";

	klee_make_symbolic(&nbcons, sizeof(nbcons),
			strcat(buffer_nbcons, polyhedronNumber));
	klee_assume(nbcons >= MIN_NBCONS & nbcons <= MAX_NBCONS & nbcons >= dim);
	ap_lincons0_array_t lincons0 = ap_lincons0_array_make(nbcons);

	for (i = 0; i < nbcons; i++) {
		ap_constyp_t type;
		klee_make_symbolic(&type, sizeof(type),
				strcat(buffer_type, polyhedronNumber));
		klee_assume(type == AP_CONS_SUPEQ | type == AP_CONS_EQ);
		lincons0.p[i].constyp = type;

		int symbolicValues[MAX_DIM + 1];
		klee_make_symbolic(symbolicValues, sizeof(symbolicValues),
				strcat(buffer_sym, polyhedronNumber));
		for (j = 0; j <= dim; j++) {
			klee_assume(symbolicValues[j] != 0);
		}
		ap_linexpr0_t * linexpr0 = create_linexpr0(dim, symbolicValues);
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

pk_t* create_polyhedron(ap_manager_t* man, pk_t* top, char * polyhedronNumber,
		unsigned short int dim) {
	ap_lincons0_array_t constraints = create_constraints(dim, polyhedronNumber);
	pk_t* polyhedron = pk_meet_lincons_array(man, false, top, &constraints);
	//printf("Created polyhedron %s!\n", polyhedronNumber);
	return polyhedron;
}

unsigned short int make_symbolic_dimension() {
	unsigned short int dim;
	klee_make_symbolic(&dim, sizeof(dim), "number of variables");
	klee_assume(dim > MIN_DIM & dim < MAX_DIM);
	return dim;
}
