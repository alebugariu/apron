#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out35.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = pk_manager_alloc(false);
		pk_t * top = pk_top(man, dim, 0);
		pk_t * bottom = pk_bottom(man, dim, 0);

		// assignment should return bottom if the current set of constraints is bottom

		int assignedToVariable;
		if (create_variable(&assignedToVariable, true, dim, data, dataSize,
				&dataIndex, fp)) {

			ap_linexpr0_t** assignmentArray;
			ap_dim_t * tdim;

			if (create_assignment(&assignmentArray, assignedToVariable, &tdim,
					dim, data, dataSize, &dataIndex, fp)) {

				pk_t* assign_result1 = pk_assign_linexpr_array(man,
				DESTRUCTIVE, bottom, tdim, assignmentArray, 1, NULL);

				if (pk_is_bottom(man, assign_result1) == false) {
					pk_free(man, top);
					pk_free(man, bottom);
					pk_free(man, assign_result1);
					free(assignmentArray);
					free(tdim);
					ap_manager_free(man);
					fclose(fp);
					return 1;
				}
				free(assignmentArray);
				free(tdim);
			}
		}
		pk_free(man, top);
		pk_free(man, bottom);
		ap_manager_free(man);
	}
	fclose(fp);
	return 0;
}

