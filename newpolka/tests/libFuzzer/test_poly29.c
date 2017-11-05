#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out29.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, bottom, dim, data, dataSize,
				&dataIndex, fp)) {
			if (assume_fuzzable(pk_is_leq(man, polyhedron1, bottom))) {
				//meet == glb, join == lub
				//x widening bottom == x
				if (!pk_is_eq(man, pk_widening(man, polyhedron1, bottom),
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

