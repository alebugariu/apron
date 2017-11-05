#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out26.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, bottom, dim, data, dataSize,
				&dataIndex, fp)) {
			pk_t* polyhedron2;
			if (create_polyhedron(&polyhedron2, man, top, bottom, dim, data, dataSize,
					&dataIndex, fp)) {
				pk_t* glb = pk_meet(man, DESTRUCTIVE, polyhedron1, polyhedron2);

				pk_t* bound;
				if (create_polyhedron(&bound, man, top, bottom, dim, data, dataSize,
						&dataIndex, fp)) {

					//meet == glb, join == lub
					//meet is the greatest lower bound
					if (assume_fuzzable(pk_is_leq(man, bound, polyhedron1))) {
						if (assume_fuzzable(
								pk_is_leq(man, bound, polyhedron2))) {
							if (!pk_is_leq(man, bound, glb)) {
								pk_free(man, top);
								pk_free(man, bottom);
								pk_free(man, polyhedron1);
								pk_free(man, polyhedron2);
								pk_free(man, glb);
								pk_free(man, bound);
								ap_manager_free(man);
								fclose(fp);
								return 1;
							}
						}
					}
					pk_free(man, glb);
					pk_free(man, bound);
				}
				pk_free(man, polyhedron2);
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
