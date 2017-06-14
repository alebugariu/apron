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
	oct_t* glb = oct_meet(man, false, octagon1, octagon2);

	oct_t* bound = create_octagon(man, top, "possible bound", dim);

	//meet == glb, join == lub
	//meet is the greatest lower bound
	klee_assume(oct_is_leq(man, bound, octagon1));
	klee_assume(oct_is_leq(man, bound, octagon2));
	klee_assert(oct_is_leq(man, bound, glb));
	return 0;
}
