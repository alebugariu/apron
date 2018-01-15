#include <time.h>
#include "oct.h"
#include "oct_internal.h"

#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out39.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		//meet == glb, join == lub
		//narrowing reaches a fixed point
		oct_t* octagon1;
		unsigned char number1;
		if (get_octagon(&octagon1, man, top, &number1, data, dataSize, &dataIndex, fp)) {
			oct_t* narrowingResult;
			int i = 0;
			while (true) {
				oct_t* octagon2;
				unsigned char number2;
				if (get_octagon(&octagon2, man, top, &number2, data, dataSize, &dataIndex, fp)) {
					narrowingResult = oct_narrowing(man, octagon1,
							octagon2);
					if (oct_is_leq(man, octagon1, narrowingResult)) {
						break; // we reached a fixed point
					}
					octagon1 = narrowingResult;
					i++;
					if (!(R(i))) {
						fprintf(fp, "found octagon %d!\n", number1);
						print_octagon(man, octagon1, number1, fp);
						fprintf(fp, "found octagon %d!\n", number2);
						print_octagon(man, octagon2, number2, fp);
						fflush(fp);
						free_pool(man);
						ap_manager_free(man);
						fclose(fp);
						return 1;
					}
				} else {
					break;
				}
			}
		}
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}

