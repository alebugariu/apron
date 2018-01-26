#include <time.h>
#include <string.h>
#include <stdio.h>
#include "test.h"

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen(create_fileName("out20.txt"), "w+");

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

				ap_abstract0_t* element3;
				unsigned char number3;
				if (get_element(&element3, man, top, &number3, data, dataSize,
						&dataIndex, fp)) {

					ap_abstract0_t* meet12 = ap_abstract0_meet(man, DESTRUCTIVE,
							element1, element2);

					ap_abstract0_t* meet12_3 = ap_abstract0_meet(man,
							DESTRUCTIVE, meet12, element3);

					ap_abstract0_t* meet23 = ap_abstract0_meet(man, DESTRUCTIVE,
							element2, element3);

					ap_abstract0_t* meet1_23 = ap_abstract0_meet(man,
							DESTRUCTIVE, element1, meet23);

					//meet == glb, join == lub
					//meet is associative
					if (ap_abstract0_is_eq(man, meet12_3, meet1_23) == false) {
						fprintf(fp, "found element %d!\n", number1);
						print_element(man, element1, number1, fp);
						fprintf(fp, "found element %d!\n", number2);
						print_element(man, element2, number2, fp);
						fprintf(fp, "found element %d!\n", number3);
						print_element(man, element3, number3, fp);
						fflush(fp);
						free_pool(man);
						free_element(man, &top);
						free_element(man, &bottom);
						free_element(man, &element1);
						free_element(man, &element2);
						free_element(man, &element3);
						ap_abstract0_free(man, meet12);
						ap_abstract0_free(man, meet12_3);
						ap_abstract0_free(man, meet23);
						ap_abstract0_free(man, meet1_23);
						ap_manager_free(man);
						fclose(fp);
						return 1;
					}
					free_element(man, &element3);
					ap_abstract0_free(man, meet12);
					ap_abstract0_free(man, meet12_3);
					ap_abstract0_free(man, meet23);
					ap_abstract0_free(man, meet1_23);
				}
				free_element(man, &element2);
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

