#include <time.h>
#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out37.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		oct_t* octagon1;
		unsigned char number1;
		if (get_octagon(&octagon1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			oct_t* octagon2;
			unsigned char number2;
			if (get_octagon(&octagon2, man, top, &number2, data, dataSize,
					&dataIndex, fp)) {

				// conditional is monotone
				if (assume_fuzzable(oct_is_leq(man, octagon1, octagon2))) {

					ap_lincons0_array_t conditionalArray;

					if (create_conditional(&conditionalArray, data, dataSize,
							&dataIndex, fp)) {

						oct_t* cond_result1 = oct_meet_lincons_array(
								man,
								DESTRUCTIVE, octagon1, &conditionalArray);

						oct_t* cond_result2 = oct_meet_lincons_array(
								man,
								DESTRUCTIVE, octagon2, &conditionalArray);

						if (oct_is_leq(man, cond_result1, cond_result2)
								== false) {
							fprintf(fp, "found octagon %d!\n", number1);
							print_octagon(man, octagon1, number1, fp);
							fflush(fp);
							fprintf(fp, "found octagon %d!\n", number2);
							print_octagon(man, octagon2, number2, fp);
							fflush(fp);
							free_pool(man);
							oct_free(man, cond_result1);
							oct_free(man, cond_result2);
							ap_manager_free(man);
							fclose(fp);
							return 1;
						}
						oct_free(man, cond_result1);
						oct_free(man, cond_result2);
					}
				}
				free_octagon(man, &octagon2);
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
