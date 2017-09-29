#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	long dim;
		FILE *fp;
		fp = fopen("out25.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);
		oct_t * bottom = oct_bottom(man, dim, 0);

		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, dim, data, dataSize,
				&dataIndex, fp)) {
			oct_t* octagon2;
			if (create_octagon(&octagon2, man, top, dim, data, dataSize,
					&dataIndex, fp)) {
				oct_t* lub = oct_join(man, false, octagon1, octagon2);

				oct_t* bound;
				if (create_octagon(&bound, man, top, dim, data, dataSize,
						&dataIndex, fp)) {
					//meet == glb, join == lub
					//join is the least upper bound
					if (assume_fuzzable(oct_is_leq(man, octagon1, bound))) {
						if (assume_fuzzable(
								oct_is_leq(man, octagon2, bound))) {
							if (!oct_is_leq(man, lub, bound)) {
								fclose(fp);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	fclose(fp);
	return 0;
}

