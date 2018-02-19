#include <time.h>
#include "box.h"
#include "oct.h"
#include "oct_fun.h"
#include "pk.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

ap_linexpr0_t * create_octogonal_linexpr0(int dim, long v1, long v2,
		double coeff1, double coeff2, double scalar_value) {
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

bool create_box(box_t ** box, ap_manager_t* man, int dim, double lowerBound,
		double upperBound, int var) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_top(intervals[i]);
	}
	ap_interval_set_double(intervals[var], lowerBound, upperBound);
	*box = box_of_box(man, dim, 0, intervals);
	return true;
}

bool create_octagon(oct_t** octagon, ap_manager_t* man, oct_t * top, int dim,
		ap_lincons0_array_t constraints) {
	*octagon = oct_meet_lincons_array(man, false, top, &constraints);
	return true;
}

bool create_polyhedron(pk_t** polyhedron, ap_manager_t* man, pk_t * top,
		int dim, ap_lincons0_array_t constraints) {
	*polyhedron = pk_meet_lincons_array(man, false, top, &constraints);
	return true;
}

int main(int argc, char **argv) {
	int dim = 8;

	//box ****************************************************************************
	ap_manager_t * box_man = box_manager_alloc();
	box_t * bo_top = box_top(box_man, dim, 0);
	box_t * bo_bottom = box_bottom(box_man, dim, 0);

	box_t* box1;

	if (create_box(&box1, box_man, dim, 10, 20, 6)) {
		printf("created box 1\n");
		ap_lincons0_array_t i1 = box_to_lincons_array(box_man, box1);
		ap_lincons0_array_fprint(stdout, &i1, NULL);
		fflush(stdout);
	}

	box_t* box2;

	if (create_box(&box2, box_man, dim, 0, 1000, 6)) {
		printf("created box 2\n");
		ap_lincons0_array_t i2 = box_to_lincons_array(box_man, box2);
		ap_lincons0_array_fprint(stdout, &i2, NULL);
		fflush(stdout);
	}

	//less equal
	printf("box1 <= box2: %d\n", box_is_leq(box_man, box1, box2));

	//meet
	box_t* box_meet_result = box_meet(box_man, false, box1, box2);

	//join
	box_t* box_join_result = box_join(box_man, false, box1, box2);

	//assign
	ap_linexpr0_t** box_assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	double box_values[9] = { -3, 7, 3, 12, -410, 0, 0, -1, 12 };
	ap_linexpr0_t* box_assignment_expression = create_polyhedral_linexpr0(dim,
			box_values); // we always use polyhedral expressions for assignments
	box_assignmentArray[0] = box_assignment_expression;
	ap_dim_t * box_tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	box_tdim[0] = 4; //the variable we want to assigned to
	box_t * box_assign_result = box_assign_linexpr_array(box_man, false, box1,
			box_tdim, box_assignmentArray, 1, NULL);

	printf("\n");

	//octagons ***********************************************************************
	ap_manager_t * oct_man = oct_manager_alloc();
	oct_t * oc_top = oct_top(oct_man, dim, 0);
	oct_t * oc_bottom = oct_bottom(oct_man, dim, 0);

	oct_t* octagon1;

	ap_lincons0_array_t oct_lincons1 = ap_lincons0_array_make(1);
	oct_lincons1.p[0].constyp = AP_CONS_SUPEQ;
	ap_linexpr0_t * oct_linexpr1 = create_octogonal_linexpr0(dim, 1, 3, 1, -1,
			4);
	oct_lincons1.p[0].linexpr0 = oct_linexpr1;

	if (create_octagon(&octagon1, oct_man, oc_top, dim, oct_lincons1)) {
		printf("created octagon 1\n");
		ap_lincons0_array_t o1 = oct_to_lincons_array(oct_man, octagon1);
		ap_lincons0_array_fprint(stdout, &o1, NULL);
		fflush(stdout);
	}

	oct_t* octagon2;

	ap_lincons0_array_t oct_lincons2 = ap_lincons0_array_make(1);
	oct_lincons2.p[0].constyp = AP_CONS_EQ;
	ap_linexpr0_t * oct_linexpr2 = create_octogonal_linexpr0(dim, 1, 3, 1, -1,
			3);
	oct_lincons2.p[0].linexpr0 = oct_linexpr2;

	if (create_octagon(&octagon2, oct_man, oc_top, dim, oct_lincons2)) {
		printf("created octagon 2\n");
		ap_lincons0_array_t o2 = oct_to_lincons_array(oct_man, octagon2);
		ap_lincons0_array_fprint(stdout, &o2, NULL);
		fflush(stdout);
	}

	//less equal
	printf("octagon2 <= octagon1: %d\n",
			oct_is_leq(oct_man, octagon2, octagon1));

	//meet
	oct_t* oct_meet_result = oct_meet(oct_man, false, octagon1, octagon2);

	//join
	oct_t* oct_join_result = oct_join(oct_man, false, octagon1, octagon2);

	//assign
	ap_linexpr0_t** oct_assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	double oct_values[9] = { 14, 0, 0, -12, 10, 1, -1, 1, 89 };
	ap_linexpr0_t* oct_assignment_expression = create_polyhedral_linexpr0(dim,
			oct_values); // we always use polyhedral expressions for assignments
	oct_assignmentArray[0] = oct_assignment_expression;
	ap_dim_t * oct_tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	oct_tdim[0] = 0; //the variable we want to assigned to
	oct_t * oct_assign_result = oct_assign_linexpr_array(oct_man, false,
			octagon1, oct_tdim, oct_assignmentArray, 1, NULL);

	printf("\n");

	//newpolka **********************************************************************
	ap_manager_t * poly_man = pk_manager_alloc(false);
	pk_t * poly_top = pk_top(poly_man, dim, 0);
	pk_t * poly_bottom = pk_bottom(poly_man, dim, 0);

	pk_t* polyhedron1;

	ap_lincons0_array_t lincons1 = ap_lincons0_array_make(1);
	lincons1.p[0].constyp = AP_CONS_SUPEQ;
	double values1[9] = { 1, 2, 0, 3, 4, -1, -1, 1, 34 };
	ap_linexpr0_t * linexpr1 = create_polyhedral_linexpr0(dim, values1);
	lincons1.p[0].linexpr0 = linexpr1;

	if (create_polyhedron(&polyhedron1, poly_man, poly_top, dim, lincons1)) {
		printf("created polyhedron 1\n");
		ap_lincons0_array_t p1 = pk_to_lincons_array(poly_man, polyhedron1);
		ap_lincons0_array_fprint(stdout, &p1, NULL);
		fflush(stdout);
	}

	pk_t* polyhedron2;

	ap_lincons0_array_t lincons2 = ap_lincons0_array_make(1);
	lincons2.p[0].constyp = AP_CONS_EQ;
	double values2[9] = { -1, 0, 3, 5, 1, 1, -1, -1, 100 };
	ap_linexpr0_t * linexpr2 = create_polyhedral_linexpr0(dim, values2);
	lincons2.p[0].linexpr0 = linexpr2;

	if (create_polyhedron(&polyhedron2, poly_man, poly_top, dim, lincons2)) {
		printf("created polyhedron 2\n");
		ap_lincons0_array_t p2 = pk_to_lincons_array(poly_man, polyhedron2);
		ap_lincons0_array_fprint(stdout, &p2, NULL);
		fflush(stdout);
	}

	//less equal
	printf("polyhedron1 <= polyhedron2: %d\n",
			pk_is_leq(poly_man, polyhedron1, polyhedron2));

	//meet
	pk_t* poly_meet_result = pk_meet(poly_man, false, polyhedron1, polyhedron2);

	//join
	pk_t* poly_join_result = pk_join(poly_man, false, polyhedron1, polyhedron2);

	//assign
	ap_linexpr0_t** assignmentArray = (ap_linexpr0_t**) malloc(
			sizeof(ap_linexpr0_t*));
	double values3[9] = { 4, 0, 43, 2, 0, 0, 1, 1, 24 };
	ap_linexpr0_t* assignment_expression = create_polyhedral_linexpr0(dim,
			values3);
	assignmentArray[0] = assignment_expression;
	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = 2; //the variable we want to assigned to
	pk_t * poly_assign_result = pk_assign_linexpr_array(poly_man, false,
			polyhedron1, tdim, assignmentArray, 1, NULL);
	return 0;
}

