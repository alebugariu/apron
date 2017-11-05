#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out30.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, bottom, dim, data, dataSize,
				&dataIndex, fp)) {
			if (assume_fuzzable(pk_is_leq(man, bottom, polyhedron1))) {
				//meet == glb, join == lub
				//bottom widening x == x
				if (!pk_is_eq(man, pk_widening(man, bottom, polyhedron1),
						polyhedron1)) {
					pk_free(man, top);
					pk_free(man, bottom);
					pk_free(man, polyhedron1);
					ap_manager_free(man);
					fclose(fp);
					return 1;
				}
			}
			pk_free(man, polyhedron1);
		}
		pk_free(man, top);
		pk_free(man, bottom);
		ap_manager_free(man);
	}
	fclose(fp);
	return 0;
}

