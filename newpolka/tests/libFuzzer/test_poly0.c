#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out0.txt", "w+");

	int dim = create_dimension(fp);

	ap_manager_t * man = pk_manager_alloc(false);
	pk_t * top = pk_top(man, dim, 0);
	pk_t * bottom = pk_bottom(man, dim, 0);

	if (create_pool(man, top, bottom, dim, data, dataSize, &dataIndex, fp)) {

		pk_t* polyhedron1;
		unsigned char number1;
		if (get_polyhedron(&polyhedron1, man, top, &number1, data, dataSize,
				&dataIndex, fp)) {
			// bottom <= x
			if (pk_is_leq(man, bottom, polyhedron1) == false) {
				fprintf(fp, "found polyhedron %d!\n", number1);
				print_polyhedron(man, polyhedron1, number1, fp);
				fflush(fp);
				free_pool(man);
				free_polyhedron(man, &top);
				free_polyhedron(man, &bottom);
				free_polyhedron(man, &polyhedron1);
				ap_manager_free(man);
				fclose(fp);
				return 1;
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
