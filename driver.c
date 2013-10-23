#include <stdio.h>
#include "math_mod.h"
#include "hamming.h"
#include "matrix.h"

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

void print_array(int *array, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf("%i", array[i]);
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
    
    printf("========== Base Matrix ===========\n");
    printf("Size of base: %i x %i\n", hamming.base->rows, hamming.base->columns);
    print_matrix(hamming.base);
    printf("========== End ===========\n");

    printf("========== Generator Matrix ===========\n");
    printf("Size of generator: %i x %i\n", hamming.generator->rows, hamming.generator->columns);
    print_matrix(hamming.generator);
    printf("========== End ===========\n");

    int user_input = -1;

    //To store the user inputed vector
    int vector[hamming.generator->columns];
    int corrected_vector[hamming.generator->columns];

    //Need to store the user inputed vector plus \0
    char buffer[hamming.generator->columns + 1];

    //Enough room to store the syndrome
    int unreduced_syndrome[hamming.generator->rows];
    //Enough room to store the syndrome
    int syndrome[hamming.generator->rows];

    //What was the error that occurred
    int error = 0;

    printf("Enter in vectors to be decoded\n");

    while (user_input != 0)
    {
    	scanf("%s", buffer);

    	for (int i = 0; i < hamming.generator->columns; i++)
    	{
    		//Make sure that we do not overflow
    		if (buffer[i] == '\0')
    		{
    			printf("Error: Vector is too short!");
    			return -1;
    		}

    		//Insert the number into vector and corrected vectored
    		vector[i] = buffer[i] - '0';
    		corrected_vector[i] = buffer[i] - '0';
    	}

    	//Decode the vector
    	hamming_decode_vector(&hamming, vector, unreduced_syndrome, syndrome, &error, corrected_vector);

    	//Print out fun facts
    	printf("Vector Received: ");
    	print_array(vector, hamming.generator->columns);
    	printf("Unreduced Syndrome: ");
    	print_array(unreduced_syndrome, hamming.generator->rows);
    	printf("Syndrome: ");
    	print_array(syndrome, hamming.generator->rows);
    	printf("Error: %i\n", error);
    	printf("Corrected Vector: ");
    	print_array(corrected_vector, hamming.generator->columns);
    }

    hamming_free(&hamming);
}
