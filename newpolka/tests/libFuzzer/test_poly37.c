#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out37.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		pk_t* polyhedron1;
		unsigned char number1;
		if (get_polyhedron(&polyhedron1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			pk_t* polyhedron2;
			unsigned char number2;
			if (get_polyhedron(&polyhedron2, man, top, &number2, data, dataSize,
					&dataIndex, fp)) {

				// conditional is monotone
				if (assume_fuzzable(
						pk_is_leq(man, polyhedron1, polyhedron2))) {

					ap_lincons0_array_t conditionalArray;

					if (create_conditional(&conditionalArray, data, dataSize,
							&dataIndex, fp)) {

						pk_t* cond_result1 =
								pk_meet_lincons_array(man,
								DESTRUCTIVE, polyhedron1, &conditionalArray);

						pk_t* cond_result2 =
								pk_meet_lincons_array(man,
								DESTRUCTIVE, polyhedron2, &conditionalArray);

						if (pk_is_leq(man, cond_result1, cond_result2)
								== false) {
							fprintf(fp, "found polyhedron %d!\n", number1);
							print_polyhedron(man, polyhedron1, number1, fp);
							fflush(fp);
							fprintf(fp, "found polyhedron %d!\n", number2);
							print_polyhedron(man, polyhedron2, number2, fp);
							fflush(fp);
							free_pool(man);
							pk_free(man, cond_result1);
							pk_free(man, cond_result2);
							ap_manager_free(man);
							fclose(fp);
							return 1;
						}
						pk_free(man, cond_result1);
						pk_free(man, cond_result2);
					}
				}
				free_polyhedron(man, &polyhedron2);
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
