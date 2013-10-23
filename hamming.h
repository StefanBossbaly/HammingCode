#ifndef HAMMING_H_
#define HAMMING_H_

#include "matrix.h"

typedef struct
{
	matrix_t *base;
	matrix_t *generator;
    int r;
    int q;
} hamming_t;


void hamming_init(hamming_t *hamming, int r, int q);
void hamming_generate_matrix(hamming_t *hamming);
void hamming_generate_base_matrix(hamming_t *hamming);
void hamming_eliminate_multiples(hamming_t *hamming);
void hamming_decode_vector(hamming_t *hamming, int *vector, int *unreduced_syndrome, int *syndrome, int *error, int *corrected_vector);
void hamming_free(hamming_t *hamming);



#endif
