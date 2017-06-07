#ifndef TEST_OCT_H_
#define TEST_OCT_H_

#include "oct.h"
#include "oct_internal.h"

#define MIN_DIM 1
#define MAX_DIM 10
#define MIN_NBCONS 1
#define MAX_NBCONS 3

oct_t* create_octagon(ap_manager_t* man, oct_t * top,
		char * octagonNumber, unsigned short int dim);

void make_symbolic_dimension(unsigned short int * dim);

#endif /* TEST_OCT_H_ */
