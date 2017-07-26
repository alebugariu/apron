#include <time.h>
#include "oct.h"
#include "oct_internal.h"
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const int *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out3.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);

		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, dim, data, dataSize, &dataIndex,
				fp)) {
			oct_t* octagon2;
			if (create_octagon(&octagon2, man, top, dim, data, dataSize,
					&dataIndex, fp)) {
				oct_t* octagon3;
				if (create_octagon(&octagon3, man, top, dim, data, dataSize,
						&dataIndex, fp)) {

					// <= is transitive
					if (assume_fuzzable(
							oct_is_leq(man, octagon1, octagon2)
									&& oct_is_leq(man, octagon2,
											octagon3))) {
						if (!oct_is_leq(man, octagon1, octagon3)) {
							fclose(fp);
							return 1;
						}
					}
				}
			}
		}
	}
	fclose(fp);
	return 0;
}
