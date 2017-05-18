#include <time.h>
#include <klee/klee.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>


bool is_join_compatible_reciprocal(ap_manager_t * man, oct_t * x, oct_t * y) {
	if (oct_is_eq(man, oct_join(man, false, x, y), y)) {
		return oct_is_leq(man, x, y);
	}
	return true;
}

int main(int argc, char **argv) {
	unsigned short int dim;
	size_t nbcons;
	klee_make_symbolic(&dim, sizeof(dim), "number of variables");
	klee_make_symbolic(&nbcons, sizeof(nbcons), "number of constraints");
	klee_assume(dim > 1);
	klee_assume(nbcons > 0);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	oct_t* octagon1 = create_octagon(man, top, "1", dim, nbcons);
	oct_t* octagon2 = create_octagon(man, top, "2", dim, nbcons);

	//meet == glb, join == lub
	//join is compatible (reciprocal)
	klee_assert(is_join_compatible_reciprocal(man, octagon1, octagon2));
	return 0;
}




