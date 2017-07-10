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

	// <= is reflexive
	klee_assert(pk_is_leq(man, polyhedron1, polyhedron1));
	return 0;
}

