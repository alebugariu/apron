#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const int *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out21.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {
		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {

			//meet == glb, join == lub
			//meet is idempotent
			if (!pk_is_eq(man,
					opt_oct_meet(man, false, polyhedron1, polyhedron1), polyhedron1)) {
				fclose(fp);
				return 1;
			}
		}
	}
	fclose(fp);
	return 0;
}
