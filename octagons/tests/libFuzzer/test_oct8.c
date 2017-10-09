#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out8.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);

		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {
			oct_t* octagon2;
			if (create_octagon(&octagon2, man, top, dim, data, dataSize,
					&dataIndex, fp)) {

				//meet == glb, join == lub
				//y <= x join y
				if (!oct_is_leq(man, octagon2,
						oct_join(man, false, octagon1, octagon2))) {
					oct_free(man, top);
					oct_free(man, octagon1);
					oct_free(man, octagon2);
					ap_manager_free(man);
					fclose(fp);
					return 1;
				}
				oct_free(man, octagon2);
			}
			oct_free(man, octagon1);
		}
		oct_free(man, top);
		ap_manager_free(man);
	}
	fclose(fp);
	return 0;
}

