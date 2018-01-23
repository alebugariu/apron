#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out36.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		pk_t* polyhedron1;
		unsigned char number1;
		if (get_polyhedron(&polyhedron1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			// assign(x) less equal project(x)

			unsigned char assignedToVariable;
			if (create_variable(&assignedToVariable, true, dim, data, dataSize,
					&dataIndex, fp)) {

				ap_linexpr0_t** assignmentArray;
				ap_dim_t * tdim;

				if (create_assignment(&assignmentArray, assignedToVariable,
						&tdim, dim, data, dataSize, &dataIndex, fp)) {

					pk_t* assign_result1 =
							pk_assign_linexpr_array(man,
							DESTRUCTIVE, polyhedron1, tdim, assignmentArray, 1,
							NULL);
					pk_internal_t * assign1_internal =
							pk_init_from_manager(man,
									AP_FUNID_ASSIGN_LINEXPR_ARRAY);

					pk_t* project_result1 = pk_forget_array(man,
					DESTRUCTIVE, polyhedron1, tdim, 1, false);

					if (assign1_internal->exn != AP_EXC_OVERFLOW) {

						if (pk_is_leq(man, assign_result1, project_result1)
								== false) {
							fprintf(fp, "found polyhedron %d!\n", number1);
							print_polyhedron(man, polyhedron1, number1, fp);
							fflush(fp);
							free_pool(man);
							free_polyhedron(man, &top);
							free_polyhedron(man, &bottom);
							free_polyhedron(man, &polyhedron1);
							pk_free(man, assign_result1);
							pk_free(man, project_result1);
							free(assignmentArray);
							free(tdim);
							ap_manager_free(man);
							fclose(fp);
							return 1;
						}
					}
					pk_free(man, assign_result1);
					pk_free(man, project_result1);
					free(assignmentArray);
					free(tdim);
				}
			}
			free_polyhedron(man, &polyhedron1);
		}
		free_polyhedron(man, &top);
		free_polyhedron(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}

