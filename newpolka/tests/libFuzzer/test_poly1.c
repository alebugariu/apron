#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const int *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim = MIN_DIM;
	FILE *fp;
	fp = fopen("out1.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {
		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);

		pk_t* polyhedron1;
		if (create_polyhedron(&polyhedron1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {

			// x <= top
			if (!pk_is_leq(man, polyhedron1, top)) {
				fclose(fp);
				return 1;
			}
		}
	}
	fclose(fp);
	return 0;

}

