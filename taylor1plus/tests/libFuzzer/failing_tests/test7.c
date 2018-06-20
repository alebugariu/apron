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
	int dim = 2;

	ap_manager_t * man = t1p_manager_alloc();
	ap_abstract0_t* bottom = ap_abstract0_bottom(man, dim, 0);
	ap_abstract0_t* top = ap_abstract0_top(man, dim, 0);

	ap_dim_t * tdim = (ap_dim_t *) malloc(sizeof(ap_dim_t));
	tdim[0] = 0;

	ap_abstract0_t* zonotope1 = ap_abstract0_forget_array(man, false, bottom,
			tdim, 1, false);
	printf("zonotope1:\n");
	ap_abstract0_fprint(stdout, man, zonotope1, NULL);
	printf("zonotope1 is bottom: %d\n", ap_abstract0_is_bottom(man, zonotope1));

	double values2[2][2] = { { 0, 0 }, { -6469, -1121 } };

	ap_abstract0_t* zonotope2 = create_zonotope(man, dim, values2);
	printf("zonotope2:\n");
	ap_abstract0_fprint(stdout, man, zonotope2, NULL);

	ap_abstract0_t* zonotope3 = ap_abstract0_join(man, false, zonotope1, zonotope2);
	printf("zonotope3:\n");
	ap_abstract0_fprint(stdout, man, zonotope3, NULL);

	printf("zonotope1 <= zonotope1 join zonotope2: ");
	printf("%d\n",
			(ap_abstract0_is_leq(man, zonotope1, zonotope3)));
	return 0;
}

