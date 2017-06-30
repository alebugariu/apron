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

	pk_t* polyhedron1 = create_polyhedron(man, top, "1", dim);
	pk_t* polyhedron2 = create_polyhedron(man, top, "2", dim);
	pk_t* polyhedron3 = create_polyhedron(man, top, "3", dim);

	// <= is transitive
	klee_assume(pk_is_leq(man, polyhedron1, polyhedron2) & pk_is_leq(man, polyhedron2, polyhedron3));
	klee_assert(pk_is_leq(man, polyhedron1, polyhedron3));
	return 0;
}

