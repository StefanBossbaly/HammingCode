/*
 * matrix.c
 *
 *  Created on: Oct 20, 2013
 *      Author: stefan
 */

#include <stdlib.h>
#include "matrix.h"

void matrix_init(matrix_t *matrix, int rows, int columns)
{
	matrix->rows = rows;
	matrix->columns = columns;


	//Create an array of pointers
    matrix->data = (int **) calloc(matrix->rows, sizeof(int *));

    for (int i = 0; i < matrix->rows; i++)
    {
        matrix->data[i] = (int *) calloc(matrix->columns, sizeof(int));
    }
}

void matrix_get_column(matrix_t *matrix, int column, int *buffer)
{
	for (int i = 0; i < matrix->rows; i++)
	{
		buffer[i] = matrix->data[i][column];
	}
}

void matrix_free(matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        free(matrix->data[i]);
    }

    free(matrix->data);
}
