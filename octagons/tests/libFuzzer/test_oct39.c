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
	fp = fopen("out39.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);
		oct_t * bottom = oct_bottom(man, dim, 0);

		//meet == glb, join == lub
		//widening reaches a fixed point
		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {
			oct_t* narrowingResult;
			int i = 0;
			while (true) {
				oct_t* octagon2;
				if (create_octagon(&octagon2, man, top, dim, data, dataSize,
						&dataIndex, fp)) {
					narrowingResult = oct_narrowing(man, octagon1,
							octagon2);
					if (oct_is_leq(man, octagon1, narrowingResult)) {
						break; // we reached a fixed point
					}
					octagon1 = narrowingResult;
					i++;
					fflush(fp);
					if (!(R(i))) {
						fclose(fp);
						return 1;
					}
				} else {
					break;
				}
			}
		}
	}
	fclose(fp);
	return 0;
}

