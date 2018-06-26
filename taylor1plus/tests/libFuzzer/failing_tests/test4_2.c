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

	ap_manager_t* man = t1p_manager_alloc();

	double values19[8][2] = { { 1258, 6705 }, { -6220, 3166 }, { -4322, 5132 },
			{ LONG_MIN, 0 }, { LONG_MIN, -1 }, { -5868, 8297 }, { 1, 1 }, {
					6281, 6855 } };

	ap_abstract0_t* zonotope19 = create_zonotope(man, dim, values19);
	printf("zonotope19:\n");
	ap_abstract0_fprint(stdout, man, zonotope19, NULL);

	double values17[8][2] = { { 0, 0 }, { -5460, 3388 }, { 8366, 9538 }, {
			LONG_MIN, 0 }, { 2542, 5322 }, { LONG_MIN, LONG_MAX },
			{ 1, LONG_MAX }, { -6451, 8909 } };

	ap_abstract0_t* zonotope17 = create_zonotope(man, dim, values17);
	printf("zonotope17:\n");
	ap_abstract0_fprint(stdout, man, zonotope17, NULL);

	ap_abstract0_t* zonotope33 = ap_abstract0_widening(man, zonotope19,
			zonotope17);
	printf("zonotope33:\n");
	ap_abstract0_fprint(stdout, man, zonotope33, NULL);

	double values10[8][2] = { { LONG_MIN, -1 }, { LONG_MIN, LONG_MIN }, { -7250,
			-920 }, { -1, 1 }, { -68, 8872 }, { -6209, 6214 }, { -1, LONG_MAX },
			{ 0, 1 } };

	ap_abstract0_t* zonotope10 = create_zonotope(man, dim, values10);
	printf("zonotope10:\n");
	ap_abstract0_fprint(stdout, man, zonotope10, NULL);

	double values0[8][2] = { { 0, 0 }, { -6469, -1121 }, { LONG_MIN, LONG_MIN },
			{ LONG_MIN, 0 }, { -2034, 8206 }, { 7806, 8741 }, { 1, LONG_MAX }, {
					4244, 5112 } };

	ap_abstract0_t* zonotope0 = create_zonotope(man, dim, values0);
	printf("zonotope0:\n");
	ap_abstract0_fprint(stdout, man, zonotope0, NULL);

	ap_abstract0_t* zonotope32 = ap_abstract0_meet(man, false, zonotope10, zonotope0);
	printf("zonotope32:\n");
	ap_abstract0_fprint(stdout, man, zonotope32, NULL);
	/*printf("zonotope1 <= zonotope2 : %d\n",
	 ap_abstract0_is_leq(man, zonotope19, zonotope2));
	 //should be false
	 printf("zonotope2 <= zonotope1 : %d\n",
	 ap_abstract0_is_leq(man, zonotope2, zonotope19));
	 printf("zonotope1 == zonotope2 : %d\n",
	 ap_abstract0_is_eq(man, zonotope19, zonotope2));*/
	return 0;
}

