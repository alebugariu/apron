#ifndef TEST_OCT_H_
#define TEST_OCT_H_

#include "oct.h"
#include "oct_internal.h"

#define MIN_DIM 1
#define MAX_DIM 20
#define MIN_NBCONS 1
#define MAX_NBCONS 30

//for widening
#define K 3
#define R(i) i < K

oct_t* create_octagon(ap_manager_t* man, oct_t * top,
		char * octagonNumber, unsigned short int dim);

unsigned short int make_symbolic_dimension();

#endif /* TEST_OCT_H_ */
