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
	oct_t * bottom = oct_bottom(man, dim, 0);

	//meet == glb, join == lub
	//widening reaches a fixed point
	oct_t* octagon1 = create_octagon(man, top, "1", dim);
	oct_t* narrowingResult;
	int i = 0;
	while (true) {
		oct_t* octagon2 = create_octagon(man, top, "2", dim);
		narrowingResult = oct_narrowing(man, octagon1, octagon2);
		if (oct_is_leq(man, octagon1, narrowingResult)) {
			break; // we reached a fixed point
		}
		octagon1 = narrowingResult;
		i++;
		klee_assert(R(i));
	}
	return 0;
}

