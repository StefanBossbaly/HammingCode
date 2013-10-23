#include "math_mod.h"
#include "hamming.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void hamming_init(hamming_t *hamming, int r, int q)
{
    hamming->r = r;
    hamming->q = q;
    
    //Allocate our pointers
    hamming->base = (matrix_t *) malloc(sizeof(matrix_t));
    hamming->generator = (matrix_t *) malloc(sizeof(matrix_t));

    //Init our matrices
    matrix_init(hamming->base, hamming->r, pow(hamming->q, hamming->r) - 1);
    matrix_init(hamming->generator, hamming->r, (pow(q, r) - 1) / (q - 1));
}

void hamming_generate_matrix(hamming_t *hamming)
{
	//First generate our base matrix
    hamming_generate_base_matrix(hamming);
    
    //Eliminate scalar multiples from our base to get our generator
    hamming_eliminate_multiples(hamming);
}

void hamming_generate_base_matrix(hamming_t *hamming)
{
	//Create an alias
	matrix_t *base = hamming->base;

	//Buffer for the columns
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
    
	//Iterate through each element in vectors a and b
    for (int i = 0; i < hamming->r; i++)
    {
    	//Make sure either a[i] or b[i] is non-zero
    	if (a[i] != 0 || b[i] != 0)
    	{
    		//Get the multiplier between a[i] and b[i]
            int temp = is_mod_multiple(a[i], b[i], hamming->q);

            //If multiple hasn't been set yet set it
            if (multiple == -1)
            {
            	multiple = temp;
            }
            //If the multiple doesn't equal temp then a and b are not scalar multiples
            else if (multiple != temp)
            {
            	return 0;
            }
    	}
    }

    //If multiple was set and is not zero (indicating that there was no multiple between a and b)
    if ((multiple != -1) && (multiple != 0))
    {
    	return multiple;
    }

    //There was no multiple between a and b
    return 0;
}

void hamming_eliminate_multiples(hamming_t *hamming)
{
	//Columns buffers for our generator and base matrix
	int base_col[hamming->base->rows];
	int gen_col[hamming->generator->rows];

	//Get the first column and add it to our generator matrix
	matrix_get_column(hamming->base, 0, base_col);
	matrix_insert_column(hamming->generator, 0, base_col);

	//Size of the generator matrix
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

		//If the columns were independent of each other
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

	//Go through each row in the generator matrix
	for (int i = 0; i < generator->rows; i++)
	{
		//Take the inner product of the i-th row of the generator matrix and the received vector
		unreduced_syndrome[i] = mod_inner_product(generator->data[i], vector, generator->columns, hamming->q);
	}

	int gen_column[generator->rows];

	//Go through each syndrome in the generator matrix
	for (int i = 0; i < generator->columns; i++)
	{
		matrix_get_column(generator, i, gen_column);

		//See the scalar multiple between the unreduced syndrome and the i-th syndrome of the generator
		int alpha = hamming_is_scalar_multiple(hamming, unreduced_syndrome, gen_column);

		//If there is an error
		if (alpha != 0)
		{
			//Set our error to alpha
			*error = alpha;

			//The reduced syndrome is the i-th syndrome of the generator
			memcpy(syndrome, gen_column, sizeof(int) * generator->rows);

			//Take i-th element of the received vector and subtract error from it
			//to get our corrected vector
			corrected_vector[i] = mod_subtract(vector[i], *error, hamming->q);

			return;
		}
	}

	//There was no error
	*error = 0;

	//Copy the unreduced_syndrome to our syndrome (it should be 000)
	memcpy(syndrome, unreduced_syndrome, sizeof(int) * generator->rows);
}

void hamming_free(hamming_t *hamming)
{
	//Free the matrices
	matrix_free(hamming->base);
	matrix_free(hamming->generator);

	//Free the pointers to the matrices
	free(hamming->base);
	free(hamming->generator);
}
