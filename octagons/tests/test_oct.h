#ifndef TEST_OCT_H_
#define TEST_OCT_H_

#include "oct.h"
#include "oct_internal.h"

oct_t* create_octagon(ap_manager_t* man, oct_t * top,
		char * octagonNumber, unsigned short int dim, size_t nbcons);


#endif /* TEST_OCT_H_ */
