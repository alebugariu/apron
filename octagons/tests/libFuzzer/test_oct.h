#ifndef TEST_OCT_H_
#define TEST_OCT_H_

#include "oct.h"
#include "oct_internal.h"

#define MIN_DIM 8
#define MAX_DIM 8
#define MIN_NBCONS 1
#define MAX_NBCONS 50

#define MAX_POOL_SIZE 32

#define NBOPS 32
#define NBASSIGNMENTS 4
#define MIN_VALUE -9999
#define MAX_VALUE 9999
#define MIN_RANDOM_VARIABLE 0
#define MAX_RANDOM_VARIABLE 15
#define VAR_THRESHOLD 12

#define OVERFLOW 3

#define FROM_POOL true
#define DESTRUCTIVE false
#define RANDOM_ASSIGNMENT false
#define RANDOM_CONDITIONAL false

#define ASSIGN 0
#define PROJECT 1
#define MEET 2
#define JOIN 3
#define WIDENING 4

//for widening
#define K 100
#define R(i) i < K

bool create_pool(ap_manager_t* man, oct_t * top, oct_t * bottom,
		int dim, const long *data, size_t dataSize, unsigned int *dataIndex,
		FILE *fp);

void free_pool(ap_manager_t* man);

bool get_octagon(oct_t** octagon, ap_manager_t* man, oct_t * top,
		unsigned char *number, const long *data, size_t dataSize,
		unsigned int *dataIndex, FILE *fp);

int create_dimension(FILE *fp);

bool create_assignment(ap_linexpr0_t*** assignmentArray,
		int assignedToVariable, ap_dim_t ** tdim, int dim, const long *data,
		size_t dataSize, unsigned int *dataIndex, FILE *fp);

bool create_conditional(ap_lincons0_array_t *conditionalArray,
		const long *data, size_t dataSize, unsigned int *dataIndex, FILE *fp);

bool create_variable(unsigned char *variable, bool assign, int dim,
		const long *data, size_t dataSize, unsigned int *dataIndex, FILE *fp);

bool assume_fuzzable(bool condition);

void print_octagon(ap_manager_t* man, oct_t* octagon, unsigned char number, FILE *fp);

#endif /* TEST_OCT_H_ */
