#ifndef TEST_OCT_H_
#define TEST_OCT_H_

#include "oct.h"
#include "oct_internal.h"

#define MIN_DIM 1
#define MAX_DIM 20
#define MIN_NBCONS 1
#define MAX_NBCONS 50

//for widening
#define K 100
#define R(i) i < K

bool create_octagon(oct_t** octagon, ap_manager_t* man, oct_t * top,
		int dim, const int *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp);

bool make_fuzzable(void *array, size_t size, const int *data, size_t dataSize,
		unsigned int *dataIndex);
bool assume_fuzzable(bool condition);

bool make_fuzzable_dimension(int * dim, const int *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp);

#endif /* TEST_OCT_H_ */
