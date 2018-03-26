#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include <string.h>
#include <stdio.h>

oct_t * create_zonotope(ap_manager_t* man, int dim, double values[dim][2]) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_double(intervals[i], values[i][0], values[i][1]);
	}
	oct_t * zonotope = oct_of_box(man, dim, 0, intervals);
	return zonotope;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t * man = oct_manager_alloc();

	double values0[8][2] = { { 0, 1 }, { 0, 0 }, { 0, LONG_MAX },
			{ LONG_MIN, 1 }, { 0, 0 }, { -1, LONG_MAX }, { 1, 0 }, { 0, 0 } };
	oct_t* zonotope0 = create_zonotope(man, dim, values0);
	printf("successfully created zonotope0:\n");
	oct_fprint(stdout, man, zonotope0, NULL);

	double values3[8][2] = { { 1, LONG_MAX }, { -1, 0 }, { LONG_MIN, 0 }, { 0, 1 }, { LONG_MIN,
			0 }, { LONG_MIN, 0 }, { 1, 7834 }, { 2117, 3121 } };
	oct_t* zonotope3 = create_zonotope(man, dim, values3);
	printf("successfully created zonotope1:\n");
	oct_fprint(stdout, man, zonotope3, NULL);

	printf("zonotope0 <= zonotope3 join zonotope0: ");
	printf("%d\n",
			(oct_is_leq(man, zonotope0,
					oct_join(man, false, zonotope3, zonotope0))));
	return 0;
}

