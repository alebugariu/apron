#include <time.h>
#include "t1p.h"
#include <string.h>
#include <stdio.h>

t1p_t * create_zonotope(ap_manager_t* man, int dim, double values[dim][2]) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_double(intervals[i], values[i][0], values[i][1]);
	}
	t1p_t * zonotope = t1p_of_box(man, dim, 0, intervals);
	return zonotope;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t* man = t1p_manager_alloc();

	double values0[8][2] = { { LONG_MIN, LONG_MAX }, { LONG_MAX, LONG_MAX }, { LONG_MIN, 0 },
			{ 0, LONG_MAX }, { LONG_MIN, 1 }, { -1, 0 }, { 0, 0 }, { LONG_MIN, 0 } };
	t1p_t* zonotope0 = create_zonotope(man, dim, values0);
	printf("successfully created zonotope0:\n");
	t1p_fprint(stdout, man, zonotope0, NULL);

	double values11[8][2] = { { -7897, -1953 }, { -8564, 3486 }, { -4587, 3373 },
			{ 5980, 9508 }, { 6900, 7560 }, { -7001, 9399 }, { -8879, 8303 }, { -8319,
					4517 } };
	t1p_t* zonotope11 = create_zonotope(man, dim, values11);
	printf("successfully created zonotope11:\n");
	t1p_fprint(stdout, man, zonotope11, NULL);

	printf("zonotope11 meet zonotope0 <= zonotope11: ");
	printf("%d\n",
			t1p_is_leq(man, t1p_meet(man, false, zonotope11, zonotope0),
					zonotope11));
	return 0;
}






