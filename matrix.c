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

void matrix_insert_column(matrix_t *matrix, int column, int *buffer)
{
	for (int i = 0; i < matrix->rows; i++)
	{
		matrix->data[i][column] = buffer[i];
	}
}

void matrix_get_column(matrix_t *matrix, int column, int *buffer)
{
	for (int i = 0; i < matrix->rows; i++)
	{
		buffer[i] = matrix->data[i][column];
	}
}

int matrix_is_equal(matrix_t *a, matrix_t *b)
{
	if ((a->rows != b->rows) || (a->columns != b->columns))
	{
		return 0;
	}

	for (int i = 0; i < a->rows; i++)
	{
		for (int j = 0; j < a->columns; j++)
		{
			if (a->data[i][j] != b->data[i][j])
			{
				return 0;
			}
		}
	}

	return 1;
}

void matrix_free(matrix_t *matrix)
{
    for (int i = 0; i < matrix->rows; i++)
    {
        free(matrix->data[i]);
    }

    free(matrix->data);
}
