#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out20.txt", "w+");

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

				pk_t* polyhedron3;
				unsigned char number3;
				if (get_polyhedron(&polyhedron3, man, top, &number3, data,
						dataSize, &dataIndex, fp)) {

					pk_t* meet12 = pk_meet(man, DESTRUCTIVE,
							polyhedron1, polyhedron2);
					pk_internal_t * meet12_internal =
							pk_init_from_manager(man, AP_FUNID_MEET);

					pk_t* meet12_3 = pk_meet(man, DESTRUCTIVE,
							meet12, polyhedron3);
					pk_internal_t * meet12_3_internal =
							pk_init_from_manager(man, AP_FUNID_MEET);

					pk_t* meet23 = pk_meet(man, DESTRUCTIVE,
							polyhedron2, polyhedron3);
					pk_internal_t * meet23_internal =
							pk_init_from_manager(man, AP_FUNID_MEET);

					pk_t* meet1_23 = pk_meet(man, DESTRUCTIVE,
							polyhedron1, meet23);
					pk_internal_t * meet1_23_internal =
							pk_init_from_manager(man, AP_FUNID_MEET);

					if (meet12_internal->exn != AP_EXC_OVERFLOW
							&& meet12_3_internal->exn != AP_EXC_OVERFLOW
							&& meet23_internal->exn != AP_EXC_OVERFLOW
							&& meet1_23_internal->exn != AP_EXC_OVERFLOW) {

						//meet == glb, join == lub
						//meet is associative
						if (pk_is_eq(man, meet12_3, meet1_23) == false) {
							fprintf(fp, "found polyhedron %d!\n", number1);
							print_polyhedron(man, polyhedron1, number1, fp);
							fprintf(fp, "found polyhedron %d!\n", number2);
							print_polyhedron(man, polyhedron2, number2, fp);
							fprintf(fp, "found polyhedron %d!\n", number3);
							print_polyhedron(man, polyhedron3, number3, fp);
							fflush(fp);
							free_pool(man);
							free_polyhedron(man, &top);
							free_polyhedron(man, &bottom);
							free_polyhedron(man, &polyhedron1);
							free_polyhedron(man, &polyhedron2);
							free_polyhedron(man, &polyhedron3);
							pk_free(man, meet12);
							pk_free(man, meet12_3);
							pk_free(man, meet23);
							pk_free(man, meet1_23);
							ap_manager_free(man);
							fclose(fp);
							return 1;
						}
					}
					free_polyhedron(man, &polyhedron3);
					pk_free(man, meet12);
					pk_free(man, meet12_3);
					pk_free(man, meet23);
					pk_free(man, meet1_23);
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

