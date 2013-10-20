#ifndef HAMMING_H_
#define HAMMING_H_

typedef struct
{
    int **generator_matrix;
    int rows;
    int columns;
    int r;
    int q;
    
} hamming_t;

void hamming_init(hamming_t *hamming, int r, int q);
void hamming_generate_matrix(hamming_t *hamming);
void hamming_generate_base_matrix(hamming_t *hamming, int **base_matrix);
void hamming_eliminate_multiples(hamming_t *hamming, int **base_matrix);



#endif