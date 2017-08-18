#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

ap_linexpr0_t * create_linexpr0(long dim, long *values) {
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

bool create_constraints(ap_lincons0_array_t *lincons0, long dim,
		const long *data, size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	size_t i, j;
	long nbcons = MIN_NBCONS;
	if (!make_fuzzable(&nbcons, sizeof(nbcons), data, dataSize, dataIndex)) //number of constraints
			{
		return false;
	}
	if (!assume_fuzzable(
			nbcons >= MIN_NBCONS && nbcons <= MAX_NBCONS && nbcons >= dim)) {
		return false;
	}
	fprintf(fp, "Number of constraints: %ld\n", nbcons);
	fflush(fp);
	*lincons0 = ap_lincons0_array_make(nbcons);
	for (i = 0; i < nbcons; i++) {
		ap_constyp_t type;
		if (!make_fuzzable(&type, sizeof(type), data, dataSize, dataIndex)) //type
				{
			return false;
		}
		if (!assume_fuzzable(
				type == AP_CONS_SUPEQ || type == AP_CONS_EQ)) {
			return false;
		}
		fprintf(fp, "Type: %c\n", type == 0 ? 'e' : 's');
		fflush(fp);
		lincons0->p[i].constyp = type;

		long fuzzableValues[MAX_DIM + 1];
		if (!make_fuzzable(fuzzableValues, (dim + 1) * sizeof(long), data,
				dataSize, dataIndex)) {
			return false;
		}
		fprintf(fp, "Values: ");
		for (j = 0; j < dim; j++) {
			fprintf(fp, "%ld, ", fuzzableValues[j]);
		}
		fprintf(fp, "%ld\n", fuzzableValues[j]);
		fflush(fp);
		ap_linexpr0_t * linexpr0 = create_linexpr0(dim, fuzzableValues);
		lincons0->p[i].linexpr0 = linexpr0;
	}
	return true;
}

bool create_polyhedron(pk_t** polyhedron, ap_manager_t* man,
		pk_t * top, long dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {
	ap_lincons0_array_t constraints;
	if (!create_constraints(&constraints, dim, data, dataSize, dataIndex, fp)) {
		return false;
	}
	*polyhedron = pk_meet_lincons_array(man, false, top, &constraints);
	return true;
}

bool make_fuzzable(void *array, size_t size, const long *data, size_t dataSize,
		unsigned int *dataIndex) {
	long numberOfElements = dataSize / sizeof(long);
	if (numberOfElements <= *dataIndex) {
		return false;
	}
	if (numberOfElements <= (*dataIndex + size)) {
		return false;
	}

	memcpy(array, data + *dataIndex, size);
	*dataIndex += size;
	return true;
}

bool assume_fuzzable(bool condition) {
	return condition;
}

bool make_fuzzable_dimension(long *dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {
	if (make_fuzzable(dim, sizeof(int), data, dataSize, dataIndex)) {
		if (assume_fuzzable(*dim > MIN_DIM && *dim < MAX_DIM)) {
			fprintf(fp, "Dim: %ld\n", *dim);
			fflush(fp);
			return true;
		}
	}
	return false;
}
