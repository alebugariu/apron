#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out38.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		// conditional should return bottom if the current set of constraints is bottom

		ap_lincons0_array_t conditionalArray;

		if (create_conditional(&conditionalArray, data, dataSize, &dataIndex,
				fp)) {

			pk_t* cond_result1 = pk_meet_lincons_array(man,
			DESTRUCTIVE, bottom, &conditionalArray);

			if (pk_is_bottom(man, cond_result1) == false) {
				ap_lincons0_array_t a1 = pk_to_lincons_array(man,
						cond_result1);
				fprintf(fp, "found non bottom conditional result: ");
				ap_lincons0_array_fprint(fp, &a1, NULL);
				fflush(fp);
				ap_lincons0_array_clear(&a1);
				free_pool(man);
				pk_free(man, cond_result1);
				ap_manager_free(man);
				fclose(fp);
				return 1;
			}
			pk_free(man, cond_result1);
		}
		free_polyhedron(man, &top);
		free_polyhedron(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
