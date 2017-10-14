#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	long dim;
	FILE *fp;
	fp = fopen("out23.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {
			pk_t* polyhedron2;
			if (create_polyhedron(&polyhedron2, man, top, dim, data, dataSize,
					&dataIndex, fp)) {

				//meet == glb, join == lub
				//meet is compatible (reciprocal)
				if (assume_fuzzable(
						pk_is_eq(man,
								pk_meet(man, DESTRUCTIVE, polyhedron1, polyhedron2),
								polyhedron1))) {
					if (!pk_is_leq(man, polyhedron1, polyhedron2)) {
						fclose(fp);
						return 1;
					}
				}
			}
		}
	}
	fclose(fp);
	return 0;
}

