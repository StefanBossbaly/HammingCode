#include <stdio.h>
#include "math_mod.h"
#include "hamming.h"
#include "matrix.h"
#include <string.h>

void print_matrix(matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        for (int j = 0; j < matrix->columns; j++)
        {
            if (j != matrix->columns - 1)
            {
                printf("%02i,", matrix->data[i][j]);
            }
            else
            {
               printf("%02i", matrix->data[i][j]); 
            }
        }

        printf("\n");
    }
}

void print_array(int *array, int length)
{
	for (int i = 0; i < length; i++)
	{
            if (i != length - 1)
            {
                printf("%02i,", array[i]);
            }
            else
            {
                printf("%02i", array[i]);
            }
	}

	printf("\n");
}

int main(void)
{
    printf("This program generates a Ham(r,q) code\n");
    printf("Please enter r and q in this format r,q\n");
    
    int r, q;
    scanf("%i, %i", &r, &q);
    
    if (!is_prime(q))
    {
        printf("q is not prime!\n");
        return -1;
    }
    
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
    char buffer[1024];

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
        
        int nums_now, bytes_now;
        int bytes_consumed = 0, nums_read = 0;
        
        while ((nums_now = sscanf(buffer + bytes_consumed, "%i,%n", vector + nums_read, &bytes_now)) > 0) {
            bytes_consumed += bytes_now;
            nums_read += nums_now;
        }
        
        memcpy(corrected_vector, vector, sizeof(int) * hamming.generator->columns);

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
