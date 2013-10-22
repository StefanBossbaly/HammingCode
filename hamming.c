#include "math_mod.h"
#include "hamming.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void hamming_init(hamming_t *hamming, int r, int q)
{
    hamming->r = r;
    hamming->q = q;
    
    hamming->base = (matrix_t *) malloc(sizeof(matrix_t));
    hamming->generator = (matrix_t *) malloc(sizeof(matrix_t));

    matrix_init(hamming->base, hamming->r, pow(hamming->q, hamming->r) - 1);
    matrix_init(hamming->generator, hamming->r, (pow(q, r) - 1) / (q - 1));
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

    if ((multiple != -1) && (multiple != 0))
    {
    	return multiple;
    }

    return 0;
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

void hamming_decode_vector(hamming_t *hamming, int *vector, int *unreduced_syndrome, int *syndrome, int *error, int *corrected_vector)
{
	//Create an alias
	matrix_t *generator = hamming->generator;

	for (int i = 0; i < generator->rows; i++)
	{
		unreduced_syndrome[i] = mod_inner_product(generator->data[i], vector, generator->columns, hamming->q);
	}

	int gen_column[generator->rows];

	for (int i = 0; i < generator->columns; i++)
	{
		matrix_get_column(generator, i, gen_column);

		int alpha = hamming_is_scalar_multiple(hamming, unreduced_syndrome, gen_column);

		if (alpha != 0)
		{
			*error = alpha;

			memcpy(syndrome, gen_column, sizeof(int) * generator->rows);

			int error_index = hamming_get_syndrome_index(hamming, syndrome);

			printf("%i\n", error_index);

			corrected_vector[error_index] = mod_subtract(vector[error_index], *error, hamming->q);

			return;
		}
	}

	*error = 0;
	memcpy(syndrome, unreduced_syndrome, sizeof(int) * generator->rows);
}

int hamming_get_syndrome_index(hamming_t *hamming, int *syndrome)
{
	int buffer[hamming->generator->rows];

	for (int i = 0; i < hamming->generator->columns; i ++)
	{
		matrix_get_column(hamming->generator, i, buffer);

		int isEqual = 1;

		for (int j = 0; j < hamming->generator->rows && isEqual; j++)
		{
			if (buffer[j] != syndrome[j])
			{
				isEqual = 0;
			}
		}

		if (isEqual)
		{
			return i;
		}
	}

	return -1;
}

void hamming_free(hamming_t *hamming)
{
	matrix_free(hamming->base);
	matrix_free(hamming->generator);

	free(hamming->base);
	free(hamming->generator);
}
