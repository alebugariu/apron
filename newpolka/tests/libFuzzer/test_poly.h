#ifndef TEST_POLY_H_
#define TEST_POLY_H_

#define MIN_DIM 1
#define MAX_DIM 20
#define MIN_NBCONS 1
#define MAX_NBCONS 50
#define MIN_NBOPS 0
#define MAX_NBOPS 5
#define MIN_VALUE -9999
#define MAX_VALUE 9999
#define MIN_RANDOM_VARIABLE 0
#define MAX_RANDOM_VARIABLE 15
#define VAR_THRESHOLD 12

#define DESTRUCTIVE false

#define FROM_TOP 1
#define WITH_ASSIGNMENT 2
#define RANDOM_PROGRAM 3
#define CONSTRUCTION_METHOD WITH_ASSIGNMENT

#define TOP 1
#define BOTTOM 2

#define ASSIGN 1
#define PROJECT 2
#define MEET 3
#define JOIN 4

//for widening
#define K 100
#define R(i) i < K

#include "pk.h"

bool create_polyhedron(pk_t** polyhedron, ap_manager_t* man, pk_t * top, pk_t * bottom,
		int dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp);

bool create_variable(int *variable, bool assign, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp);

bool create_assignment(ap_linexpr0_t*** assignmentArray,
		int assignedToVariable, ap_dim_t** tdim, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp);

bool make_fuzzable(void *array, size_t size, const long *data, size_t dataSize,
		unsigned int *dataIndex);

bool assume_fuzzable(bool condition);

bool make_fuzzable_dimension(int * dim, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp);

#endif /* TEST_POLY_H_ */
