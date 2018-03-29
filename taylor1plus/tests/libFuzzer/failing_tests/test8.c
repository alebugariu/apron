#include <time.h>
#include "t1p.h"
#include "box.h"
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

box_t * create_box(ap_manager_t* man, int dim, double values[dim][2]) {
	ap_interval_t** intervals = ap_interval_array_alloc(dim);
	int i;
	for (i = 0; i < dim; i++) {
		ap_interval_set_double(intervals[i], values[i][0], values[i][1]);
	}
	box_t * box = box_of_box(man, dim, 0, intervals);
	return box;
}

int main(int argc, char **argv) {
	int dim = 8;

	ap_manager_t* man = t1p_manager_alloc();

	double values0[8][2] = { { 0, 1 }, { 0, 0 }, { 0, LONG_MAX },
			{ LONG_MIN, 1 }, { 0, 0 }, { -1, LONG_MAX }, { 1, 0 }, { 0, 0 } };
	t1p_t* zonotope0 = create_zonotope(man, dim, values0);
	printf("successfully created zonotope0:\n");
	t1p_fprint(stdout, man, zonotope0, NULL);

	double values3[8][2] = { { 1, LONG_MAX }, { -1, 0 }, { LONG_MIN, 0 },
			{ 0, 1 }, { LONG_MIN, 0 }, { LONG_MIN, 0 }, { 1, 7834 }, { 2117,
					3121 } };
	t1p_t* zonotope3 = create_zonotope(man, dim, values3);
	printf("successfully created zonotope3:\n");
	t1p_fprint(stdout, man, zonotope3, NULL);

	printf("zonotope0 is bottom: %d\n", t1p_is_bottom(man, zonotope0));

	ap_manager_t* box_man = box_manager_alloc();
	box_t* box0 = create_box(box_man, dim, values0);
	printf("box0 is bottom: %d\n", box_is_bottom(box_man, box0));

	printf("zonotope0 <= zonotope3 join zonotope0: ");
	printf("%d\n",
			(t1p_is_leq(man, zonotope0,
					t1p_join(man, false, zonotope3, zonotope0))));
	return 0;
}

