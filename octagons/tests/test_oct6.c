#include <time.h>
#include <klee/klee.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	unsigned short int dim;
	make_symbolic_dimension(&dim);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);

	oct_t* octagon1 = create_octagon(man, top, "1", dim);

	//meet == glb, join == lub
	klee_assert(oct_is_eq(man, oct_join(man, false, top, octagon1), top));
	return 0;
}

