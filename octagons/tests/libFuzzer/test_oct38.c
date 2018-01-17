#include <time.h>
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out38.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		// conditional should return bottom if the current set of constraints is bottom

		ap_lincons0_array_t conditionalArray;

		if (create_conditional(&conditionalArray, data, dataSize, &dataIndex,
				fp)) {

			oct_t* cond_result1 = oct_meet_lincons_array(man,
			DESTRUCTIVE, bottom, &conditionalArray);

			if (oct_is_bottom(man, cond_result1) == false) {
				ap_lincons0_array_t a1 = oct_to_lincons_array(man,
						cond_result1);
				fprintf(fp, "found non bottom conditional result: ");
				ap_lincons0_array_fprint(fp, &a1, NULL);
				fflush(fp);
				ap_lincons0_array_clear(&a1);
				free_pool(man);
				oct_free(man, cond_result1);
				ap_manager_free(man);
				fclose(fp);
				return 1;
			}
		}
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
