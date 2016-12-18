//
// Created by Aman LaChapelle on 12/18/16.
//
// homeAutomation
// Copyright (c) 2016 Aman LaChapelle
// Full license at homeAutomation/LICENSE.txt
//

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "../include/matrix.h"

matrix_t *newMatrix(size_t rows, size_t cols, double *data){
  matrix_t *out = malloc(sizeof(matrix_t));
  out->rows = rows;
  out->cols = cols;

  out->data = calloc(rows*cols, sizeof(double)); //initialize out->data to zeros
  if (data){
#pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++){
      for (int j = 0; j < cols; j++){
        out->data[cols*i + j] = data[cols*i + j]; //set the output data equal to what we gave the function
      }
    }
  }

  return out;
}

void deleteMatrix(matrix_t *matrix){
  free(matrix->data);
  free(matrix);
}

double matElement(matrix_t *matrix, size_t i, size_t j){
  return matrix->data[i*matrix->rows + j];
}

vector_t *matMul(matrix_t *matrix, vector_t *vector){
  assert(matrix->cols == vector->len);

  vector_t *out = newVector(matrix->rows, NULL);

  size_t rows = matrix->rows;
  size_t cols = matrix->cols;

#pragma omp parallel for collapse(2)
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){
      out->data[i] += matElement(matrix,i,j)*vector->data[j];
    }
  }

  return out;
}
