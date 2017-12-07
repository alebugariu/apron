#include <time.h>
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	int dim;
	FILE *fp;
	fp = fopen("out49.txt", "w+");

	if (make_fuzzable_dimension(&dim, data, dataSize, &dataIndex, fp)) {

		ap_manager_t * man = oct_manager_alloc();
		oct_t * top = oct_top(man, dim, 0);
		oct_t * bottom = oct_bottom(man, dim, 0);

		oct_t* octagon1;
		if (create_octagon(&octagon1, man, top, bottom, dim, data, dataSize,
				&dataIndex, fp)) {

			// conditional(x) less equal x

			ap_lincons0_array_t conditionalExpression;

			if (create_conditional(&conditionalExpression, dim, data, dataSize,
					&dataIndex, fp)) {

				oct_t* cond_result1 = oct_meet_lincons_array(man,
				DESTRUCTIVE, bottom, &conditionalExpression);

				if (oct_is_leq(man, cond_result1, octagon1) == false) {
					oct_free(man, top);
					oct_free(man, bottom);
					oct_free(man, octagon1);
					oct_free(man, cond_result1);
					ap_manager_free(man);
					fclose(fp);
					return 1;
				}
				oct_free(man, cond_result1);
			}
			oct_free(man, octagon1);
		}
		oct_free(man, top);
		oct_free(man, bottom);
		ap_manager_free(man);
	}
	fclose(fp);
	return 0;
}

