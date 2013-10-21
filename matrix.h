/*
 * matrix.h
 *
 *  Created on: Oct 20, 2013
 *      Author: stefan
 */

#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct
{
	int **data;
	int rows;
	int columns;
} matrix_t;

void matrix_init(matrix_t *matrix, int rows, int columns);
void matrix_insert_column(matrix_t *matrix, int column, int *buffer);
void matrix_get_column(matrix_t *matrix, int column, int *buffer);
void matrix_free(matrix_t *matrix);

#endif /* MATRIX_H_ */
