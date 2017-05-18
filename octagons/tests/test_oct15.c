#include <time.h>
#include <klee/klee.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

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

	//meet == glb, join == lub
	klee_assert(oct_is_eq(man, oct_meet(man, false, bottom, octagon1), bottom));
	return 0;
}




