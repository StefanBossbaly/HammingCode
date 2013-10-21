#include <stdio.h>
#include "math_mod.h"
#include "hamming.h"
#include "matrix.h"

void print_buffer(int *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%i", buffer[i]);
    }
    
    printf("\n");
}

void print_matrix(matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
        {
            printf("%i", matrix->data[i][j]);
        }

        printf("\n");
    }
}

int main(void)
{
    printf("This program generates a Ham(r,q) code\n");
    printf("Please enter r and q in this format r,q\n");
    
    int r, q;
    scanf("%i, %i", &r, &q);
    
    hamming_t hamming;
    
    hamming_init(&hamming, r, q);
    
    hamming_generate_matrix(&hamming);
    
    printf("========== Base Matrix ===========\n");
    print_matrix(hamming.base);
    printf("========== End ===========\n");

    printf("========== Generator Matrix ===========\n");
    print_matrix(hamming.generator);
    printf("========== End ===========\n");
}
