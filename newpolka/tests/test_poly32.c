#include <time.h>
#include <klee/klee.h>
#include "pk_internal.h"
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	unsigned short int dim = make_symbolic_dimension();

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t* top = pk_top(man, dim, 0);
	pk_t* bottom = pk_bottom(man, dim, 0);

	//meet == glb, join == lub
	//widening reaches a fixed point
	pk_t* polyhedron1 = create_polyhedron(man, top, "1", dim);
	pk_t* wideningResult;
	int i = 0;
	while (true) {
		pk_t* polyhedron2 = create_polyhedron(man, top, "2", dim);
		wideningResult = pk_widening(man, polyhedron1, pk_join(man, false, polyhedron1, polyhedron2));
		if (pk_is_leq(man, wideningResult, polyhedron1)) {
			break; // we reached a fixed point
		}
		polyhedron1 = wideningResult;
		i++;
		klee_assert(R(i));
	}
	return 0;
}

