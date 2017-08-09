#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const int *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
		FILE *fp;
		fp = fopen("out20.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, dim, data, dataSize,
				&dataIndex, fp)) {
			pk_t* polyhedron2;
			if (create_polyhedron(&polyhedron2, man, top, dim, data, dataSize,
					&dataIndex, fp)) {
				pk_t* polyhedron3;
				if (create_polyhedron(&polyhedron3, man, top, dim, data, dataSize,
						&dataIndex, fp)) {

					//meet == glb, join == lub
					//meet is associative
					if (!pk_is_eq(man,
							pk_meet(man, false,
									pk_meet(man, false, polyhedron1,
											polyhedron2), polyhedron3),
							pk_meet(man, false, polyhedron1,
									pk_meet(man, false, polyhedron2,
											polyhedron3)))) {
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

