#include "math_mod.h"
#include "hamming.h"
#include <math.h>
#include <stdlib.h>

void hamming_init(hamming_t *hamming, int r, int q)
{
    hamming->r = r;
    hamming->q = q;
    
    hamming->base = (matrix_t *) malloc(sizeof(matrix_t));
    hamming->generator = (matrix_t *) malloc(sizeof(matrix_t));

    matrix_init(hamming->base, hamming->r, pow(hamming->q, hamming->r) - 1);
    matrix_init(hamming->generator, hamming->r, (pow(q, r) - 1) / (r - 1));
}

void hamming_generate_matrix(hamming_t *hamming)
{
    hamming_generate_base_matrix(hamming);
    
    for (int i = 0; i < hamming->base->rows; i++)
    {
        for (int j = 0; j < hamming->base->columns; j++)
        {
            printf("%i", hamming->base->data[i][j]);
        }
        printf("\n");
    }
    
    /*hamming_eliminate_multiples(hamming, base_matrix);
    
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%i", hamming->generator_matrix[i][j]);
        }
        printf("\n");
    }*/
}

void hamming_generate_base_matrix(hamming_t *hamming)
{
	matrix_t *base = hamming->base;

    int temp[base->rows];
    
    //Count from 0 to q^r - 1
    for (int i = 0; i < base->columns; i++)
    {
    	//Convert i+1 to a number in our mod
        convert_to_mod(i + 1, hamming->q, temp, hamming->r);
        
        //Copy the value into our base matrix
        for (int j = 0; j < base->rows; j++)
        {
            base->data[j][i] = temp[j];
        }
    }
}

int hamming_is_scalar_multiple(hamming_t *hamming, int *a, int *b)
{
    int multiple = 0;
    
    for (int i = 0; i < hamming->r; i++)
    {
        int temp = is_mod_multiple(a[i], b[i], hamming->q);
        
        if (temp == 0)
            return 0;
        else if (multiple == 0)
            multiple = temp;
        else if (multiple != temp)
            return 0;
    }
    
    return 1;
}

/*void hamming_eliminate_multiples(hamming_t *hamming, int **base_matrix)
{
    int cols = pow(hamming->q, hamming->r) - 1;
    int currentSize = 1;
    
    for (int i = 0; i < hamming->r; i++)
    {
        hamming->generator_matrix[0][i] = base_matrix[0][i];
    }
    
    int buffer1[hamming->r];
    int buffer2[hamming->r];
    
    for (int col = 0; col < cols; col++)
    {
        for (int gen = 0; gen < currentSize; gen++)
        {
            for (int row = 0; row < hamming->r; row++)
            {
                buffer1[row] = base_matrix[col][row];
                buffer2[row] = hamming->generator_matrix[gen][row];
            }
            
            if (! hamming_is_scalar_multiple(hamming, buffer1, buffer2))
            {
                for (int i = 0; i < hamming->r; i++)
                {
                    hamming->generator_matrix[currentSize][i] = base_matrix[col][i];
                }
                
                currentSize++;
            }
        }
    }
}*/
