#include <time.h>
#include <klee/klee.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	unsigned short int dim = make_symbolic_dimension();

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	oct_t* octagon1 = create_octagon(man, top, "1", dim);
	oct_t* octagon2 = create_octagon(man, top, "2", dim);
	oct_t* octagon3 = create_octagon(man, top, "3", dim);

	//meet == glb, join == lub
	//join is associative
	klee_assert(
			oct_is_eq(man,
					oct_join(man, false,
							oct_join(man, false, octagon1, octagon2), octagon3),
					oct_join(man, false, octagon1,
							oct_join(man, false, octagon2, octagon3))));
	return 0;
}
