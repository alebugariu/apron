#include <time.h>
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

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);
		oct_t * bottom = oct_bottom(man, dim, 0);

		oct_t* octagon1;
		unsigned char number1;
		if (get_octagon(&octagon1, man, top, &number1, data, dataSize, &dataIndex, fp)) {

			// conditional(x) less equal x

			ap_lincons0_array_t conditionalArray;

			if (create_conditional(&conditionalArray, data, dataSize,
					&dataIndex, fp)) {

				oct_t* cond_result1 = oct_meet_lincons_array(man,
				DESTRUCTIVE, bottom, &conditionalArray);

				if (oct_is_leq(man, cond_result1, octagon1) == false) {
					fprintf(fp, "found octagon %d!\n", number1);
					print_octagon(man, octagon1, number1, fp);
					fflush(fp);
					free_pool(man);
					oct_free(man, cond_result1);
					ap_manager_free(man);
					fclose(fp);
					return 1;
				}
			}
		}
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
