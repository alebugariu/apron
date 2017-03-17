#include <time.h>
#include <klee/klee.h>
#include <string.h>
#include "oct_internal.h"
#include "oct.h"
#include "num.h"


ap_linexpr0_t * create_linexpr0(unsigned short int dim, int v1, int v2, int coeff1, int coeff2, char * octagonNumber){
	ap_coeff_t *cst, *coeff;
	ap_linexpr0_t * linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
	cst = &linexpr0->cst;

	char buffer_scalar[80] = "scalar value for octagon ";
	int scalar_value;
	klee_make_symbolic(&scalar_value, sizeof(scalar_value), strcat(buffer_scalar, octagonNumber));
	//otherwise scalar_value is concretized to 0, because klee cannot deal with floats
	klee_assume(scalar_value >= dim & scalar_value < 10);
	ap_scalar_reinit(cst->val.scalar, AP_SCALAR_DOUBLE);
	cst->val.scalar->val.dbl = scalar_value;

	ap_linterm_t * linterm = &linexpr0->p.linterm[0];
	linterm->dim = v1;
	coeff = &linterm->coeff;
	ap_scalar_reinit(coeff->val.scalar, AP_SCALAR_DOUBLE);
	coeff->val.scalar->val.dbl = coeff1;

	linterm = &linexpr0->p.linterm[1];
	linterm->dim = v2;
	coeff = &linterm->coeff;
	ap_scalar_reinit(coeff->val.scalar, AP_SCALAR_DOUBLE);
	coeff->val.scalar->val.dbl = coeff2;
	return linexpr0;
}

ap_lincons0_array_t create_constraints(unsigned short int dim, size_t nbcons, char * octagonNumber){
	size_t i;
	ap_lincons0_array_t  lincons0 = ap_lincons0_array_make(nbcons);
	char buffer_type[80] = "type for octagon ";
	char buffer_v1[80] = "v1 for octagon ";
	char buffer_v2[80] = "v2 for octagon ";
	char buffer_coeff1[80] = "coeff1 for octagon ";
	char buffer_coeff2[80] = "coeff2 for octagon ";
	for(i=0; i < nbcons; i++){
		ap_constyp_t type;
		klee_make_symbolic(&type, sizeof(type), strcat(buffer_type, octagonNumber));
		klee_assume(type == AP_CONS_SUPEQ);
		lincons0.p[i].constyp = type;
		int v1, v2;
		klee_make_symbolic(&v1, sizeof(v1), strcat(buffer_v1, octagonNumber));
		klee_make_symbolic(&v2, sizeof(v2), strcat(buffer_v2, octagonNumber));
		klee_assume(v1 < dim & v2 < dim & v1 != v2 & v1 >=0 & v2 >= 0);
		int coeff1, coeff2;
		klee_make_symbolic(&coeff1, sizeof(coeff1), strcat(buffer_coeff1, octagonNumber));
	    klee_make_symbolic(&coeff2, sizeof(coeff2), strcat(buffer_coeff2, octagonNumber));
        klee_assume(coeff1 == 1 | coeff1 == -1);
        klee_assume(coeff2 == 1 | coeff2 == -1);
		ap_linexpr0_t * linexpr0 = create_linexpr0(dim,v1,v2,coeff1,coeff2, octagonNumber);
		lincons0.p[i].linexpr0 = linexpr0;
	}
	return lincons0;
}

oct_t* create_octagon(ap_manager_t* man, oct_t * top, char * octagonNumber, unsigned short int dim, size_t nbcons) {
	ap_lincons0_array_t constraints = create_constraints(dim, nbcons, octagonNumber);
	ap_lincons0_array_fprint(stdout,&constraints, NULL);
	oct_t* octagon = oct_meet_lincons_array(man, false, top, &constraints);
	char buffer[80] = "octagon ";
    printf("After meet with top\n");
	klee_assume(!oct_is_bottom(man, octagon));
	printf("Created not bottom octagon %s!\n", octagonNumber);
	ap_lincons0_array_fprint(stdout,&constraints,NULL);
	return octagon;
}

int main(int argc, char **argv){
	unsigned short int dim;
	size_t nbcons;
	klee_make_symbolic(&dim, sizeof(dim), "number of variables");
	klee_make_symbolic(&nbcons, sizeof(nbcons), "number of constraints");
	klee_assume(dim >= 3 & dim < 10);
	// otherwise klee_assume(v1 < dim & v2 < dim & v1 != v2 & v1 >=0 & v2 >= 0); is provably false, because the value of dim is concretized
	klee_assume(nbcons > 0 & nbcons < 10);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	oct_t* octagon1 = create_octagon(man, top, "1", dim, nbcons);
	//klee_assert(oct_is_leq(man, bottom, octagon1));
	//klee_assert(oct_is_leq(man, octagon1, top));

	//meet == glb, join == lub
	/*klee_assert(oct_is_eq(man, oct_join(man, false, bottom, octagon1),octagon1));
	klee_assert(oct_is_eq(man, oct_join(man, false, top, octagon1), top));

	oct_t* octagon2 = create_octagon(man, top, "2", dim, nbcons);
	klee_assert(oct_is_leq(man, octagon2 , oct_join(man, false, octagon1, octagon2)));
	klee_assert(oct_is_leq(man, oct_meet(man, false, octagon1, octagon2), octagon1));*/
	return 0;
}
