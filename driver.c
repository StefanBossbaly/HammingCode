#include <stdio.h>
#include "math_mod.h"
#include "hamming.h"

void print_buffer(int *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%i", buffer[i]);
    }
    
    printf("\n");
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
    
}