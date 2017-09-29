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
		fp = fopen("out20.txt", "w+");

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
				oct_t* octagon3;
				if (create_octagon(&octagon3, man, top, dim, data, dataSize,
						&dataIndex, fp)) {

					//meet == glb, join == lub
					//meet is associative
					if (!oct_is_eq(man,
							oct_meet(man, false,
									oct_meet(man, false, octagon1,
											octagon2), octagon3),
							oct_meet(man, false, octagon1,
									oct_meet(man, false, octagon2,
											octagon3)))) {
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

