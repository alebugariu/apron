#include <time.h>
#include "test_poly.h"
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

bool create_number_of_constraints(long *nbcons, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	if (!make_fuzzable(nbcons, sizeof(nbcons), data, dataSize, dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(
			*nbcons >= MIN_NBCONS && *nbcons <= MAX_NBCONS && *nbcons >= dim)) {
		return false;
	}
	fprintf(fp, "Number of constraints: %ld\n", *nbcons);
	fflush(fp);
	return true;
}

bool create_a_constraint(ap_linexpr0_t** constraint, ap_constyp_t *type,
		int dim, unsigned char randomVariable, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	size_t i;
	if (!make_fuzzable(type, sizeof(*type), data, dataSize, dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(*type == AP_CONS_SUPEQ || *type == AP_CONS_EQ)) {
		return false;
	}
	fprintf(fp, "Type: %c\n", *type == AP_CONS_EQ ? 'e' : 's');
	fflush(fp);

	long fuzzableValues[MAX_DIM + 1];
	if (!make_fuzzable(fuzzableValues, (dim + 1) * sizeof(long), data, dataSize,
			dataIndex)) {
		return false;
	}
	fprintf(fp, "Values: ");
	fflush(fp);
	for (i = 0; i < dim; i++) {
		if (!assume_fuzzable(
				!(randomVariable <= VAR_THRESHOLD)
						&& (fuzzableValues[i] >= MIN_VALUE
								&& fuzzableValues[i] <= MAX_VALUE))) {
			return false;
		}
		fprintf(fp, "%ld, ", fuzzableValues[i]);
		fflush(fp);
	}
	if (!assume_fuzzable(
			!(randomVariable <= VAR_THRESHOLD)
					&& (fuzzableValues[i] >= MIN_VALUE
							&& fuzzableValues[i] <= MAX_VALUE))) {
		return false;
	}
	fprintf(fp, "%ld\n", fuzzableValues[i]);
	fflush(fp);
	*constraint = create_linexpr0(dim, fuzzableValues);
	return true;
}

bool create_constraints(ap_lincons0_array_t *lincons0, long nbcons, int dim,
		unsigned char randomVariable, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {
	*lincons0 = ap_lincons0_array_make(nbcons);
	ap_linexpr0_t* constraint;
	ap_constyp_t type;
	size_t i;
	for (i = 0; i < nbcons; i++) {
		if (!create_a_constraint(&constraint, &type, dim, randomVariable, data,
				dataSize, dataIndex, fp)) {
			return false;
		}
		lincons0->p[i].constyp = type;
		lincons0->p[i].linexpr0 = constraint;
	}
	return true;
}

bool create_random_variable(unsigned char *randomVariable, int dim,
		const long *data, size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	if (!make_fuzzable(randomVariable, sizeof(unsigned char), data, dataSize,
			dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(
			*randomVariable >= MIN_RANDOM_VARIABLE
					&& *randomVariable <= MAX_RANDOM_VARIABLE)) {
		return false;
	}
	return true;
}

bool create_polyhedron_from_top(pk_t** polyhedron, ap_manager_t* man,
		pk_t * top, int dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {
	long nbcons;
	if (!create_number_of_constraints(&nbcons, dim, data, dataSize, dataIndex,
			fp)) {
		return false;
	}

	unsigned char randomVariable;
	if (!create_random_variable(&randomVariable, dim, data, dataSize, dataIndex,
			fp)) {
		return false;
	}

	ap_lincons0_array_t constraints;
	if (!create_constraints(&constraints, nbcons, dim, randomVariable, data,
			dataSize, dataIndex, fp)) {
		return false;
	}
	*polyhedron = pk_meet_lincons_array(man, DESTRUCTIVE, top, &constraints);
	ap_lincons0_array_clear(&constraints);
	return true;
}

bool create_polyhedron_from_bottom(pk_t** polyhedron, ap_manager_t* man,
		pk_t * top, pk_t * bottom, int dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {
	*polyhedron = bottom;

	long nbcons;
	if (!create_number_of_constraints(&nbcons, dim, data, dataSize, dataIndex,
			fp)) {
		return false;
	}

	unsigned char randomVariable;
	if (!create_random_variable(&randomVariable, dim, data, dataSize, dataIndex,
			fp)) {
		return false;
	}

	ap_lincons0_array_t constraints;
	if (!create_constraints(&constraints, nbcons, dim, randomVariable, data,
			dataSize, dataIndex, fp)) {
		return false;
	}

	size_t i;
	for (i = 0; i < nbcons; i++) {
		ap_lincons0_array_t a_constraint = ap_lincons0_array_make(1);
		a_constraint.p[0].constyp = constraints.p[i].constyp;
		a_constraint.p[0].linexpr0 = constraints.p[i].linexpr0;

		pk_t* meet_result = pk_meet_lincons_array(man,
		DESTRUCTIVE, top, &a_constraint);

		*polyhedron = pk_join(man, DESTRUCTIVE, *polyhedron, meet_result);
	}
	ap_lincons0_array_clear(&constraints);
	return true;
}

bool create_polyhedron_with_assignment(pk_t** polyhedron, ap_manager_t* man,
		pk_t * top, pk_t * bottom, int dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {

	int top_or_bottom;
	if (!make_fuzzable(&top_or_bottom, sizeof(int), data, dataSize,
			dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(top_or_bottom == TOP || top_or_bottom == BOTTOM)) {
		return false;
	}

	bool result;
	switch (top_or_bottom) {
	case TOP: {
		fprintf(fp, "Started with top!\n");
		fflush(fp);
		result = create_polyhedron_from_top(polyhedron, man, top, dim, data,
				dataSize, dataIndex, fp);
		if (result == false) {
			return false;
		}
		break;
	}
	case BOTTOM: {
		fprintf(fp, "Started with bottom!\n");
		fflush(fp);
		result = create_polyhedron_from_bottom(polyhedron, man, top, bottom,
				dim, data, dataSize, dataIndex, fp);
		if (result == false) {
			return false;
		}
		break;
	}
	}

	int nbops;
	if (!make_fuzzable(&nbops, sizeof(int), data, dataSize, dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(nbops >= MIN_NBOPS && nbops <= MAX_NBOPS)) {
		return false;
	}

	fprintf(fp, "Number of operators: %d\n", nbops);
	fflush(fp);
	size_t i;
	for (i = 0; i < nbops; i++) {
		int operator;
		if (!make_fuzzable(&operator, sizeof(int), data, dataSize, dataIndex)) {
			return false;
		}
		if (!assume_fuzzable(operator == ASSIGN || operator == PROJECT)) {
			return false;
		}

		switch (operator) {
		case ASSIGN: {
			fprintf(fp, "ASSIGN\n");
			fflush(fp);
			int assignedToVariable;
			if (!create_variable(&assignedToVariable, true, dim, data, dataSize,
					dataIndex, fp)) {
				return false;
			}
			ap_linexpr0_t** assignmentArray;
			ap_dim_t * tdim;

			if (!create_assignment(&assignmentArray, assignedToVariable, &tdim,
					dim, data, dataSize, dataIndex, fp)) {
				return false;
			}
			pk_t* assign_result = pk_assign_linexpr_array(man,
			DESTRUCTIVE, *polyhedron, tdim, assignmentArray, 1, NULL);
			*polyhedron = assign_result;
			free(assignmentArray);
			free(tdim);
			break;
		}
		case PROJECT: {
			fprintf(fp, "PROJECT\n");
			fflush(fp);
			int projectedVariable;
			if (!create_variable(&projectedVariable, false, dim, data, dataSize,
					dataIndex, fp)) {
				return false;
			}
			ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
			tdim[0] = projectedVariable;
			pk_t* project_result = pk_forget_array(man,
			DESTRUCTIVE, *polyhedron, tdim, 1, false);
			*polyhedron = project_result;
			free(tdim);
			break;
		}
		}
	}
	fprintf(fp, "Successfully created!\n");
	fflush(fp);
	return true;
}

bool create_polyhedron_as_random_program(pk_t** polyhedron,
		ap_manager_t* man, pk_t * top, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp) {

	*polyhedron = top;

	int nbops;
	if (!make_fuzzable(&nbops, sizeof(int), data, dataSize, dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(nbops >= MIN_NBOPS + 1 && nbops <= MAX_NBOPS)) {
		return false;
	}

	fprintf(fp, "Number of operators: %d\n", nbops);
	fflush(fp);
	size_t i;
	for (i = 0; i < nbops; i++) {
		int operator;
		if (!make_fuzzable(&operator, sizeof(int), data, dataSize, dataIndex)) {
			return false;
		}
		if (!assume_fuzzable(
				operator == ASSIGN || operator == PROJECT || operator == MEET
						|| operator == JOIN)) {
			return false;
		}

		switch (operator) {
		case ASSIGN: {
			fprintf(fp, "ASSIGN\n");
			fflush(fp);
			int assignedToVariable;
			if (!create_variable(&assignedToVariable, true, dim, data, dataSize,
					dataIndex, fp)) {
				return false;
			}
			ap_linexpr0_t** assignmentArray;
			ap_dim_t * tdim;

			if (!create_assignment(&assignmentArray, assignedToVariable, &tdim,
					dim, data, dataSize, dataIndex, fp)) {
				return false;
			}
			pk_t* assign_result = pk_assign_linexpr_array(man,
			DESTRUCTIVE, *polyhedron, tdim, assignmentArray, 1,
			NULL);

			*polyhedron = assign_result;
			free(assignmentArray);
			free(tdim);
			break;
		}
		case PROJECT: {
			fprintf(fp, "PROJECT\n");
			fflush(fp);
			int projectedVariable;
			if (!create_variable(&projectedVariable, false, dim, data, dataSize,
					dataIndex, fp)) {
				return false;
			}
			ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
			tdim[0] = projectedVariable;
			pk_t* project_result = pk_forget_array(man,
			DESTRUCTIVE, *polyhedron, tdim, 1, false);
			*polyhedron = project_result;
			free(tdim);
			break;
		}
		case MEET: {
			fprintf(fp, "MEET\n");
			fflush(fp);
			pk_t *other;
			if (!create_polyhedron_from_top(&other, man, top, dim, data,
					dataSize, dataIndex, fp)) {
				return false;
			}
			*polyhedron = pk_meet(man, DESTRUCTIVE, *polyhedron, other);
			break;
		}
		case JOIN: {
			fprintf(fp, "JOIN\n");
			fflush(fp);
			pk_t *other;
			if (!create_polyhedron_from_top(&other, man, top, dim, data,
					dataSize, dataIndex, fp)) {
				return false;
			}
			*polyhedron = pk_join(man, DESTRUCTIVE, *polyhedron, other);
			break;
		}
		}
	}
	fprintf(fp, "Successfully created!\n");
	fflush(fp);
	return true;
}

bool create_polyhedron(pk_t** polyhedron, ap_manager_t* man,
		pk_t * top, pk_t * bottom, int dim,
		const long *data, size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	switch (CONSTRUCTION_METHOD) {
	case FROM_TOP:
		return create_polyhedron_from_top(polyhedron, man, top, dim, data,
				dataSize, dataIndex, fp);
	case WITH_ASSIGNMENT:
		return create_polyhedron_with_assignment(polyhedron, man, top, bottom,
				dim, data, dataSize, dataIndex, fp);
	case RANDOM_PROGRAM:
		return create_polyhedron_as_random_program(polyhedron, man, top, dim,
				data, dataSize, dataIndex, fp);
	}
	return false;
}

bool create_variable(int *variable, bool assign, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	if (!make_fuzzable(variable, sizeof(int), data, dataSize, dataIndex)) {
		return false;
	}
	if (!assume_fuzzable(*variable >= 0 && *variable < dim)) {
		return false;
	}
	if (assign) {
		fprintf(fp, "Assigned to variable: %d\n", *variable);
		fflush(fp);
	} else {
		fprintf(fp, "Projected variable: %d\n", *variable);
		fflush(fp);
	}
	fflush(fp);
	return true;
}

bool create_assignment(ap_linexpr0_t*** assignmentArray,
		int assignedToVariable, ap_dim_t ** tdim, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp) {
	unsigned char randomVariable;
	if (!create_random_variable(&randomVariable, dim, data, dataSize, dataIndex,
			fp)) {
		return false;
	}

	long fuzzableValues[MAX_DIM + 1];
	if (!make_fuzzable(fuzzableValues, (dim + 1) * sizeof(long), data, dataSize,
			dataIndex)) {
		return false;
	}
	int j;
	fprintf(fp, "Assignment expression: ");
	fflush(fp);
	for (j = 0; j < dim; j++) {
		if (!assume_fuzzable(
				!(randomVariable <= VAR_THRESHOLD)
						&& (fuzzableValues[j] >= MIN_VALUE
								&& fuzzableValues[j] <= MAX_VALUE))) {
			return false;
		}
		fprintf(fp, "%ld, ", fuzzableValues[j]);
		fflush(fp);
	}
	if (!assume_fuzzable(
			!(randomVariable <= VAR_THRESHOLD)
					&& (fuzzableValues[j] >= MIN_VALUE
							&& fuzzableValues[j] <= MAX_VALUE))) {
		return false;
	}
	fprintf(fp, "%ld\n", fuzzableValues[j]);
	fflush(fp);

	ap_linexpr0_t* expression = create_linexpr0(dim, fuzzableValues);
	*assignmentArray = (ap_linexpr0_t**) malloc(sizeof(ap_linexpr0_t*));
	*assignmentArray[0] = expression;

	*tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	*tdim[0] = assignedToVariable;
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

bool make_fuzzable_dimension(int *dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp) {
	if (make_fuzzable(dim, sizeof(int), data, dataSize, dataIndex)) {
		if (assume_fuzzable(*dim > MIN_DIM && *dim < MAX_DIM)) {
			fprintf(fp, "Dim: %d\n", *dim);
			fflush(fp);
			return true;
		}
	}
	return false;
}
