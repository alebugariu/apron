#include <time.h>
#include "oct.h"
#include "oct_internal.h"

#include "test_oct.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out11.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = oct_manager_alloc();
	oct_t * top = oct_top(man, dim, 0);
	oct_t * bottom = oct_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		oct_t* octagon1;
		unsigned char number1;
		if (get_octagon(&octagon1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {

			//meet == glb, join == lub
			//join is idempotent
			if (!oct_is_eq(man,
					oct_join(man, DESTRUCTIVE, octagon1, octagon1),
					octagon1)) {
				fprintf(fp, "found octagon %d!\n", number1);
				print_octagon(man, octagon1, number1, fp);
				fflush(fp);
				free_pool(man);
				free_octagon(man, &top);
				free_octagon(man, &bottom);
				free_octagon(man, &octagon1);
				ap_manager_free(man);
				fclose(fp);
				return 1;
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

