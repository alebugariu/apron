#include <time.h>
#include "test_poly.h"
#include <string.h>
#include <stdio.h>

extern int LLVMFuzzerTestOneInput(const long *data, size_t dataSize) {
	unsigned int dataIndex = 0;
	FILE *fp;
	fp = fopen("out25.txt", "w+");

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

				pk_t* lub = pk_join(man, DESTRUCTIVE, polyhedron1,
						polyhedron2);
				unsigned char number3;
				pk_internal_t * lub_internal = pk_init_from_manager(man,
						ELINA_FUNID_JOIN);

				if (lub_internal->exn != ELINA_EXC_OVERFLOW) {

					pk_t* bound;
					if (get_polyhedron(&bound, man, top, &number3, data,
							dataSize, &dataIndex, fp)) {
						//meet == glb, join == lub
						//join is the least upper bound
						if (assume_fuzzable(
								pk_is_leq(man, polyhedron1, bound))) {
							if (assume_fuzzable(
									pk_is_leq(man, polyhedron2, bound))) {
								if (pk_is_leq(man, lub, bound) == false) {
									fprintf(fp, "found polyhedron %d!\n",
											number1);
									print_polyhedron(man, polyhedron1, number1,
											fp);
									fprintf(fp, "found polyhedron %d!\n",
											number2);
									print_polyhedron(man, polyhedron2, number2,
											fp);
									fprintf(fp, "found bound %d!\n", number3);
									print_polyhedron(man, bound, number3, fp);
									fflush(fp);
									free_pool(man);
									free_polyhedron(man, &top);
									free_polyhedron(man, &bottom);
									free_polyhedron(man, &polyhedron1);
									free_polyhedron(man, &polyhedron2);
									free_polyhedron(man, &bound);
									pk_free(man, lub);
									ap_manager_free(man);
									fclose(fp);
									return 1;
								}
							}
						}
						free_polyhedron(man, &bound);
					}
				}
				free_polyhedron(man, &polyhedron2);
				pk_free(man, lub);
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

