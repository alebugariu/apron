#include <time.h>
#include "t1p.h"
#include <string.h>
#include <stdio.h>

ap_abstract0_t * create_zonotope(ap_manager_t* man, int dim,
		double values[dim][2]) {
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

	ap_manager_t * man = t1p_manager_alloc();

	double values0[8][2] = { { 0, 1 }, { 0, 0 }, { 0, LONG_MAX },
				{ LONG_MIN, 1 }, { 0, 0 }, { -1, LONG_MAX }, { 1, 0 }, { 0, 0 } };
	ap_abstract0_t* zonotope0 = create_octagon(man, dim, values0);
	printf("successfully created zonotope0:\n");
	ap_abstract0_fprint(stdout, man, zonotope0, NULL);

	double values1[8][2] = { { 0, 0 }, { NAN, 0 }, { LONG_MIN, 1 }, { 0, 1 }, { 1,
				NAN }, { 0, LONG_MAX }, { NAN, 0 }, { NAN, LONG_MAX } };
	ap_abstract0_t* zonotope1 = create_octagon(man, dim, values1);
	printf("successfully created zonotope1:\n");
	ap_abstract0_fprint(stdout, man, zonotope1, NULL);

	ap_abstract0_t * join_result = ap_abstract0_join(man, false, zonotope0, zonotope1);
	printf("zonotope0 <= zonotope0 join zonotope1: ");
	printf("%d\n", (ap_abstract0_is_leq(man, zonotope0, join_result)));
	return 0;
}

