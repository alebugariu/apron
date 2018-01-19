#include <time.h>
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out36.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		oct_t* octagon1;
		unsigned char number1;
		if (get_octagon(&octagon1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			// assign(x) less equal project(x)

			unsigned char assignedToVariable;
			if (create_variable(&assignedToVariable, true, dim, data, dataSize,
					&dataIndex, fp)) {

				ap_linexpr0_t** assignmentArray;
				ap_dim_t * tdim;

				if (create_assignment(&assignmentArray, assignedToVariable,
						&tdim, dim, data, dataSize, &dataIndex, fp)) {

					oct_t* assign_result1 = oct_assign_linexpr_array(
							man,
							DESTRUCTIVE, octagon1, tdim, assignmentArray, 1,
							NULL);

					oct_t* project_result1 = oct_forget_array(man,
					DESTRUCTIVE, octagon1, tdim, 1, false);

					if (oct_is_leq(man, assign_result1, project_result1)
							== false) {
						fprintf(fp, "found octagon %d!\n", number1);
						print_octagon(man, octagon1, number1, fp);
						fflush(fp);
						free_pool(man);
						free_octagon(man, &top);
						free_octagon(man, &bottom);
						free_octagon(man, &octagon1);
						oct_free(man, assign_result1);
						oct_free(man, project_result1);
						free(assignmentArray);
						free(tdim);
						ap_manager_free(man);
						fclose(fp);
						return 1;
					}
					oct_free(man, assign_result1);
					oct_free(man, project_result1);
					free(assignmentArray);
					free(tdim);
				}
			}
			free_octagon(man, &octagon1);
		}
		free_octagon(man, &top);
		free_octagon(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
