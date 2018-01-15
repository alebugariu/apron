#include <time.h>
#include "oct.h"
#include "oct_internal.h"

#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out26.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		oct_t* octagon1;
		unsigned char number1;
		if (get_octagon(&octagon1, man, top, &number1, data, dataSize, &dataIndex, fp)) {

			oct_t* octagon2;
			unsigned char number2;
			if (get_octagon(&octagon2, man, top, &number2, data, dataSize, &dataIndex, fp)) {

				oct_t* glb = oct_meet(man, DESTRUCTIVE, octagon1,
						octagon2);

				oct_t* bound;
				unsigned char number3;
				if (get_octagon(&bound, man, top, &number3, data, dataSize, &dataIndex, fp)) {

					//meet == glb, join == lub
					//meet is the greatest lower bound
					if (assume_fuzzable(oct_is_leq(man, bound, octagon1))) {
						if (assume_fuzzable(
								oct_is_leq(man, bound, octagon2))) {
							if (!oct_is_leq(man, bound, glb)) {
								fprintf(fp, "found octagon %d!\n", number1);
								print_octagon(man, octagon1, number1, fp);
								fprintf(fp, "found octagon %d!\n", number2);
								print_octagon(man, octagon2, number2, fp);
								fprintf(fp, "found bound %d!\n", number3);
								print_octagon(man, bound, number3, fp);
								fflush(fp);
								free_pool(man);
								ap_manager_free(man);
								fclose(fp);
								return 1;
							}
						}
					}
				}
			}
		}
	}
	ap_manager_free(man);
	fclose(fp);
	return 0;
}
