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
	int dim = 2;

	ap_manager_t* man = t1p_manager_alloc();

	double values1[2][2] = { { 0, 0 }, { -6469,-1121 }};
	ap_abstract0_t* zonotope1 = create_zonotope(man, dim, values1);
	ap_abstract0_fprint(stdout, man, zonotope1, NULL);

	double values2[2][2] = { {-14, 2885 }, { 1, 7 }};

	ap_abstract0_t* zonotope2 = create_zonotope(man, dim, values2);
	ap_abstract0_fprint(stdout, man, zonotope2, NULL);

	ap_abstract0_t* zonotope12 = ap_abstract0_join(man, false, zonotope1,
			zonotope2);

	ap_abstract0_t* zonotope21 = ap_abstract0_join(man, false, zonotope2, zonotope1);
	printf("Join12 result:\n");
	ap_abstract0_fprint(stdout, man, zonotope12, NULL);
	printf("Join21 result:\n");
	ap_abstract0_fprint(stdout, man, zonotope21, NULL);
	printf("join12 = join21:%d\n", ap_abstract0_is_eq(man, zonotope12, zonotope21));
	return 0;
}

