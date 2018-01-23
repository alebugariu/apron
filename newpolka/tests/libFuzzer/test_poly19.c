#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out19.txt", "w+");

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

				pk_t* meet12 = pk_meet(man, DESTRUCTIVE,
						polyhedron1, polyhedron2);
				pk_internal_t * meet12_internal = pk_init_from_manager(
						man, AP_FUNID_MEET);
				pk_t* meet21 = pk_meet(man, DESTRUCTIVE,
						polyhedron2, polyhedron1);
				pk_internal_t * meet21_internal = pk_init_from_manager(
						man, AP_FUNID_MEET);

				if (meet12_internal->exn != AP_EXC_OVERFLOW
						&& meet21_internal->exn != AP_EXC_OVERFLOW) {

					//meet == glb, join == lub
					//meet is commutative
					if (pk_is_eq(man, meet12, meet21) == false) {
						fprintf(fp, "found polyhedron %d!\n", number1);
						print_polyhedron(man, polyhedron1, number1, fp);
						fprintf(fp, "found polyhedron %d!\n", number2);
						print_polyhedron(man, polyhedron2, number2, fp);
						fflush(fp);
						free_pool(man);
						free_polyhedron(man, &top);
						free_polyhedron(man, &bottom);
						free_polyhedron(man, &polyhedron1);
						free_polyhedron(man, &polyhedron2);
						pk_free(man, meet12);
						pk_free(man, meet21);
						ap_manager_free(man);
						fclose(fp);
						return 1;
					}
				}
				free_polyhedron(man, &polyhedron2);
				pk_free(man, meet12);
				free_polyhedron(man, &meet21);
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

