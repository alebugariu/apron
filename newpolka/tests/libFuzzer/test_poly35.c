#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out35.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		// assignment should return bottom if the current set of constraints is bottom

		unsigned char assignedToVariable;
		if (create_variable(&assignedToVariable, true, dim, data, dataSize,
				&dataIndex, fp)) {

			ap_linexpr0_t** assignmentArray;
			ap_dim_t * tdim;

			if (create_assignment(&assignmentArray, assignedToVariable, &tdim,
					dim, data, dataSize, &dataIndex, fp)) {

				pk_t* assign_result1 = pk_assign_linexpr_array(
						man,
						DESTRUCTIVE, bottom, tdim, assignmentArray, 1,
						NULL);
				pk_internal_t * assign1_internal = pk_init_from_manager(
						man, ELINA_FUNID_ASSIGN_LINEXPR_ARRAY);

				if (assign1_internal->exn != ELINA_EXC_OVERFLOW) {

					if (pk_is_bottom(man, assign_result1) == false) {
						ap_lincons0_array_t a1 = pk_to_lincons_array(
								man, assign_result1);
						fprintf(fp, "found non-bottom assignment result: ");
						ap_lincons0_array_fprint(fp, &a1, NULL);
						fflush(fp);
						ap_lincons0_array_clear(&a1);
						free_pool(man);
						free_polyhedron(man, &top);
						free_polyhedron(man, &bottom);
						pk_free(man, assign_result1);
						free(assignmentArray);
						free(tdim);
						ap_manager_free(man);
						fclose(fp);
						return 1;
					}
				}
				pk_free(man, assign_result1);
				free(assignmentArray);
				free(tdim);
			}
		}
		free_polyhedron(man, &top);
		free_polyhedron(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}

