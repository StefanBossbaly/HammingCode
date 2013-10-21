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
    
    hamming_eliminate_multiples(hamming);
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
        matrix_insert_column(base, i, temp);
    }
}

int hamming_is_scalar_multiple(hamming_t *hamming, int *a, int *b)
{
	int multiple = -1;
    
    for (int i = 0; i < hamming->r; i++)
    {
    	if (a[i] != 0 || b[i] != 0)
    	{
            int temp = is_mod_multiple(a[i], b[i], hamming->q);

            if (multiple == -1)
            {
            	multiple = temp;
            }
            else if (multiple != temp)
            {
            	return 0;
            }
    	}
    }

    return (multiple != -1) && (multiple != 0);
}

void hamming_eliminate_multiples(hamming_t *hamming)
{
	int base_col[hamming->base->rows];
	int gen_col[hamming->generator->rows];

	//Get the first column and add it to our generator matrix
	matrix_get_column(hamming->base, 0, base_col);
	matrix_insert_column(hamming->generator, 0, base_col);

	int size = 1;

	//Start at column 1 and go thru them all
	for (int i = 1; i < hamming->base->columns; i++)
	{
		//Get the i-th column of the base
		matrix_get_column(hamming->base, i, base_col);

		int dependant = 0;

		//Go thru each of the linearly dependant columns
		for (int j = 0; j < size && !dependant; j++)
		{
			//Get the j-th column of the generator matrix
			matrix_get_column(hamming->generator, j, gen_col);

			//If the columns are not scalar multiplies
			if (hamming_is_scalar_multiple(hamming, base_col, gen_col))
			{
				dependant = 1;
			}
		}

		if (dependant == 0)
		{
			//Insert the base column into the generator
			matrix_insert_column(hamming->generator, size, base_col);

			//Increment size
			size++;
		}
	}
}
