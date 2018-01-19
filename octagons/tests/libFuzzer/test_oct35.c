#include <time.h>
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out35.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		// assignment should return bottom if the current set of constraints is bottom

		unsigned char assignedToVariable;
		if (create_variable(&assignedToVariable, true, dim, data, dataSize,
				&dataIndex, fp)) {

			ap_linexpr0_t** assignmentArray;
			ap_dim_t * tdim;

			if (create_assignment(&assignmentArray, assignedToVariable, &tdim,
					dim, data, dataSize, &dataIndex, fp)) {

				oct_t* assign_result1 = oct_assign_linexpr_array(man,
				DESTRUCTIVE, bottom, tdim, assignmentArray, 1,
				NULL);

				if (oct_is_bottom(man, assign_result1) == false) {
					ap_lincons0_array_t a1 = oct_to_lincons_array(man,
							assign_result1);
					fprintf(fp, "found non-bottom assignment result: ");
					ap_lincons0_array_fprint(fp, &a1, NULL);
					fflush(fp);
					ap_lincons0_array_clear(&a1);
					free_pool(man);
					free_octagon(man, &top);
					free_octagon(man, &bottom);
					oct_free(man, assign_result1);
					free(assignmentArray);
					free(tdim);
					ap_manager_free(man);
					fclose(fp);
					return 1;
				}
				oct_free(man, assign_result1);
				free(assignmentArray);
				free(tdim);
			}
		}
		free_octagon(man, &top);
		free_octagon(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
