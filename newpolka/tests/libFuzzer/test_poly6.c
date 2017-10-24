#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out6.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, dim, data, dataSize,
				&dataIndex, fp)) {

			//meet == glb, join == lub
			//x join top == top
			if (!pk_is_eq(man, pk_join(man, DESTRUCTIVE, top, polyhedron1),
					top)) {
				pk_free(man, top);
				pk_free(man, polyhedron1);
				ap_manager_free(man);
				fclose(fp);
				return 1;
			}
			pk_free(man, polyhedron1);
		}
		pk_free(man, top);
		ap_manager_free(man);
	}
	fclose(fp);
	return 0;
}

