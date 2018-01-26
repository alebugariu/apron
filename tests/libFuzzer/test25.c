#include <time.h>
#include <string.h>
#include <stdio.h>
#include "test.h"

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen(create_fileName("out25.txt"), "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = create_manager();
	ap_abstract0_t * top = ap_abstract0_top(man, dim, 0);
	ap_abstract0_t * bottom = ap_abstract0_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		ap_abstract0_t* element1;
		unsigned char number1;
		if (get_element(&element1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			ap_abstract0_t* element2;
			unsigned char number2;
			if (get_element(&element2, man, top, &number2, data, dataSize,
					&dataIndex, fp)) {

				ap_abstract0_t* lub = ap_abstract0_join(man, DESTRUCTIVE,
						element1, element2);
				unsigned char number3;

				ap_abstract0_t* bound;
				if (get_element(&bound, man, top, &number3, data, dataSize,
						&dataIndex, fp)) {
					//meet == glb, join == lub
					//join is the least upper bound
					if (assume_fuzzable(
							ap_abstract0_is_leq(man, element1, bound))) {
						if (assume_fuzzable(
								ap_abstract0_is_leq(man, element2, bound))) {
							if (ap_abstract0_is_leq(man, lub, bound) == false) {
								fprintf(fp, "found element %d!\n", number1);
								print_element(man, element1, number1, fp);
								fprintf(fp, "found element %d!\n", number2);
								print_element(man, element2, number2, fp);
								fprintf(fp, "found bound %d!\n", number3);
								print_element(man, bound, number3, fp);
								fflush(fp);
								free_pool(man);
								free_element(man, &top);
								free_element(man, &bottom);
								free_element(man, &element1);
								free_element(man, &element2);
								free_element(man, &bound);
								ap_abstract0_free(man, lub);
								ap_manager_free(man);
								fclose(fp);
								return 1;
							}
						}
						free_element(man, &bound);
					}
				}
				free_element(man, &element2);
				ap_abstract0_free(man, lub);
			}
			free_element(man, &element1);
		}
		free_element(man, &top);
		free_element(man, &bottom);
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}

