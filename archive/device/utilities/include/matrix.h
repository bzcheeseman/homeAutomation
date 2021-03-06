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


#ifndef HOMEAUTOMATION_MATRIX_H
#define HOMEAUTOMATION_MATRIX_H

#include <stdlib.h>

#include "vector.h"

typedef struct {
  double *data;
  size_t rows;
  size_t cols;
} matrix_t ;

matrix_t *newMatrix(size_t rows, size_t cols, double *data);

void deleteMatrix(matrix_t *matrix);

double matElement(matrix_t *matrix, size_t i, size_t j);

vector_t *matMul(matrix_t *matrix, vector_t *vector);

#endif //HOMEAUTOMATION_MATRIX_H
