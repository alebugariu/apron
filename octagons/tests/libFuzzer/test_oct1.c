#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const int *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim = MIN_DIM;
	FILE *fp;
	fp = fopen("out1.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {
		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);

		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {

			// x <= top
			if (!oct_is_leq(man, octagon1, top)) {
				fclose(fp);
				return 1;
			}
		}
	}
	fclose(fp);
	return 0;

}

