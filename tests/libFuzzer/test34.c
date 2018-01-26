#include <time.h>
#include <string.h>
#include <stdio.h>
#include "test.h"

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen(create_fileName("out34.txt"), "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = create_manager();
	ap_abstract0_t * top = ap_abstract0_top(man, dim, 0);
	ap_abstract0_t * bottom = ap_abstract0_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		ap_abstract0_t* element1;
		unsigned char number1;
		if (get_element(&element1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			if (ap_abstract0_is_bottom(man, element1) == false) {

				// assignment cannot return bottom if the current set of constraints is not bottom

				unsigned char assignedToVariable;
				if (create_variable(&assignedToVariable, true, dim, data,
						dataSize, &dataIndex, fp)) {
					ap_linexpr0_t** assignmentArray;
					ap_dim_t * tdim;

					if (create_assignment(&assignmentArray, assignedToVariable,
							&tdim, dim, data, dataSize, &dataIndex, fp)) {

						ap_abstract0_t* assign_result1 =
								ap_abstract0_assign_linexpr_array(man,
										DESTRUCTIVE, element1, tdim,
										assignmentArray, 1, NULL);

						if (ap_abstract0_is_bottom(man, assign_result1)
								== true) {
							fprintf(fp, "found element %d!\n", number1);
							print_element(man, element1, number1, fp);
							fflush(fp);
							free_pool(man);
							free_element(man, &top);
							free_element(man, &bottom);
							free_element(man, &element1);
							ap_abstract0_free(man, assign_result1);
							free(assignmentArray);
							free(tdim);
							ap_manager_free(man);
							fclose(fp);
							return 1;
						}
						ap_abstract0_free(man, assign_result1);
						free(assignmentArray);
						free(tdim);
					}
				}
				free_element(man, &element1);
			}
		}
		free_element(man, &top);
		free_element(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}

