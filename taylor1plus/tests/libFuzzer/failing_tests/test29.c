#include <time.h>
#include "t1p.h"
#include <string.h>
#include <stdio.h>

ap_abstract0_t * create_zonotope(ap_manager_t* man, int dim, double values[dim][2]) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_double(intervals[i], values[i][0], values[i][1]);
	}
	ap_abstract0_t * zonotope = ap_abstract0_of_box(man, dim, 0, intervals);
	return zonotope;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t* man = t1p_manager_alloc();
	ap_abstract0_t * bottom = ap_abstract0_bottom(man, dim, 0);
	ap_abstract0_t * top = ap_abstract0_top(man, dim, 0);

	double values19[8][2] = { { 1258,6705 }, { -6220,3166 }, { -4322,5132 }, {
			-9202,-6307 }, { 186,7132 }, { -5868,8297 }, { -9562,3192 }, {
					6281,6855 } };

	ap_abstract0_t* zonotope19 = create_zonotope(man, dim, values19);
	printf("successfully created zonotope23:\n");
	ap_abstract0_fprint(stdout, man, zonotope19, NULL);

	double values0[8][2] = { { 0, 0 }, { 0, 1 }, { LONG_MIN, LONG_MIN }, {
			LONG_MIN, 0 }, { 0, 1 }, { 0, LONG_MAX }, { 1, LONG_MAX }, { 0,
			LONG_MAX } };
	ap_abstract0_t* zonotope0 = create_zonotope(man, dim, values0);
	printf("successfully created zonotope0:\n");
	ap_abstract0_fprint(stdout, man, zonotope0, NULL);
	/*
	(0) := [-oo,6705]	;[-oo,6705]	[-oo,6705]
	(1) := -1527 + 4693.(eta0)	;[-6220,3166]	[-6220,3166]
	(2) := [-oo,5132]	;[-oo,5132]	[-oo,5132]
	(3) := [-oo,+oo]	;[-oo,+oo]	[-oo,+oo]
	(4) := [-oo,7132]	;[-oo,7132]	[-oo,7132]
	(5) := [-5868,+oo]	;[-5868,+oo]	[-5868,+oo]
	(6) := [-9562,+oo]	;[-9562,+oo]	[-9562,+oo]
	(7) := [-oo,+oo]	;[-oo,+oo]	[-oo,+oo]
	interval of dim (0,0):

*/

	ap_abstract0_t * widening_result = ap_abstract0_widening(man, zonotope19, zonotope0);
	ap_abstract0_fprint(stdout, man, widening_result, NULL);
	printf("zonotope0 <= zonotope19 widening zonotope0: %d\n",
			ap_abstract0_is_leq(man, zonotope0, widening_result));

	return 0;
}

