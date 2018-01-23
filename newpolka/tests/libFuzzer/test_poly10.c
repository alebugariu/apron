#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out10.txt", "w+");

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

					pk_t* join12 = pk_join(man, DESTRUCTIVE,
							polyhedron1, polyhedron2);
					pk_internal_t * join12_internal =
							pk_init_from_manager(man, AP_FUNID_JOIN);

					pk_t* join23 = pk_join(man, DESTRUCTIVE,
							polyhedron2, polyhedron3);
					pk_internal_t * join23_internal =
							pk_init_from_manager(man, AP_FUNID_JOIN);

					pk_t* join12_3 = pk_join(man, DESTRUCTIVE,
							join12, polyhedron3);
					pk_internal_t * join12_3_internal =
							pk_init_from_manager(man, AP_FUNID_JOIN);

					pk_t* join1_23 = pk_join(man, DESTRUCTIVE,
							polyhedron1, join23);
					pk_internal_t * join1_23_internal =
							pk_init_from_manager(man, AP_FUNID_JOIN);

					if (join12_internal->exn != AP_EXC_OVERFLOW
							&& join23_internal->exn != AP_EXC_OVERFLOW
							&& join12_3_internal->exn != AP_EXC_OVERFLOW
							&& join1_23_internal->exn != AP_EXC_OVERFLOW) {

						//meet == glb, join == lub
						//join is associative
						if (pk_is_eq(man, join12_3, join1_23) == false) {
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
							pk_free(man, join12);
							pk_free(man, join12_3);
							pk_free(man, join23);
							pk_free(man, join1_23);
							ap_manager_free(man);
							fclose(fp);
							return 1;
						}
					}
					free_polyhedron(man, &polyhedron3);
					pk_free(man, join12);
					pk_free(man, join12_3);
					pk_free(man, join23);
					pk_free(man, join1_23);
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

