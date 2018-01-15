#include <time.h>
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out41.txt", "w+");

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
			if (oct_is_bottom(man, octagon1) == false) {

				// assignment cannot return bottom if the current set of constraints is not bottom

				unsigned char assignedToVariable;
				if (create_variable(&assignedToVariable, true, dim, data,
						dataSize, &dataIndex, fp)) {
					ap_linexpr0_t** assignmentArray;
					ap_dim_t * tdim;

					if (create_assignment(&assignmentArray, assignedToVariable,
							&tdim, dim, data, dataSize, &dataIndex, fp)) {

						oct_t* assign_result1 =
								oct_assign_linexpr_array(man, DESTRUCTIVE,
										octagon1, tdim, assignmentArray, 1,
										NULL);

						if (oct_is_bottom(man, assign_result1) == true) {
							fprintf(fp, "found octagon %d!\n", number1);
							print_octagon(man, octagon1, number1, fp);
							fflush(fp);
							free_pool(man);
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
			}
		}
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
