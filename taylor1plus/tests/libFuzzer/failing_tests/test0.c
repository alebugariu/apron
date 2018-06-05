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

	double values1[8][2] = { { 0, LONG_MAX }, { 0, LONG_MAX }, { 1, LONG_MAX },
			{ 0, LONG_MAX }, { 1, LONG_MAX }, { LONG_MIN, 0 }, { -1, 0 }, { -1,
					0 } };
	ap_abstract0_t* zonotope1 = create_zonotope(man, dim, values1);
	ap_abstract0_fprint(stdout, man, zonotope1, NULL);

	double values2[8][2] = { { -1, LONG_MAX }, { LONG_MIN, 0 }, { LONG_MIN, 0 },
			{ LONG_MIN, LONG_MIN }, { LONG_MIN, 1 }, { LONG_MIN, -1 }, { 0, 1 },
			{ 0, LONG_MAX } };
	ap_abstract0_t* zonotope2 = create_zonotope(man, dim, values2);
	ap_abstract0_fprint(stdout, man, zonotope2, NULL);

	ap_abstract0_t* zonotope3 = ap_abstract0_meet(man, false, zonotope2,
			zonotope1);

	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = 4;

	ap_abstract0_t* zonotope4 = ap_abstract0_forget_array(man, false, zonotope3,
			tdim, 1, false);
	printf("Project result:\n");
	ap_abstract0_fprint(stdout, man, zonotope4, NULL);

	ap_abstract0_free(man, zonotope3);
	return 0;
}

