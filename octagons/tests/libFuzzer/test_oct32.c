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
	fp = fopen("out32.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);
		oct_t * bottom = oct_bottom(man, dim, 0);

		//meet == glb, join == lub
		//widening reaches a fixed point
		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {
			oct_t* wideningResult;
			int i = 0;
			while (true) {
				oct_t* octagon2;
				if (create_octagon(&octagon2, man, top, dim, data, dataSize,
						&dataIndex, fp)) {
					wideningResult = oct_widening(man, octagon1, octagon2);
					if (oct_is_leq(man, wideningResult, octagon1)) {
						break; // we reached a fixed point
					}
					octagon1 = wideningResult;
					i++;
					if (!(R(i))) {
						oct_free(man, top);
						oct_free(man, bottom);
						oct_free(man, octagon1);
						oct_free(man, octagon2);
						ap_manager_free(man);
						fclose(fp);
						return 1;
					}
				} else {
					break;
				}
			}
			oct_free(man, octagon1);
		}
		oct_free(man, top);
		oct_free(man, bottom);
		ap_manager_free(man);
	}
	fclose(fp);
	return 0;
}

