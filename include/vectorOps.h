//
// Created by Aman LaChapelle on 8/21/16.
//

/*
    homeAutomation
    Copyright (C) 2016  Aman LaChapelle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 */

#ifndef HOMEAUTOMATION_VECTOROPS_H
#define HOMEAUTOMATION_VECTOROPS_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

/**
 * @file vectorOps.h
 * @brief Holds all the operations needed for a vector object - new, resize, copy, etc.
 *
 * Functions for creating a new vector, resizing it, copying it, printing it to the stdout, and freeing all its memory.
 */

/**
 * @struct vector_t
 * @brief My implementation of an analagous vector to the C++ STL container
 *
 * Holds:
 *  - The actual data in the vector
 *  - The length of the vector, to avoid counting each time
 * 
 * Note that we can represent a square matrix in its flattened form as a vector with
 * the index representation m(i,j) == v[i+m_cols*j], and with the length set to 
 * m_rows*m_cols = m_rows^2
 */
typedef struct {
  //!The data in the vector - the numbers (in the form of a double(!))
  double *data;
  //!The length of the vector
  size_t len;
} vector_t ;

/**
 * Returns a newly constructed vector object.  Of course this can be done manually, but this is easier.
 *
 * @param len Length of the vector to create
 * @param data The data (if any) to initialize the vector with.  Defaults to zero if this parameter is NULL
 * @return A new vector object
 */
vector_t *newVector(size_t len, double *data);

/**
 * Resizes a vector to a length given by newlen.
 *
 * @param vec The vector to resize
 * @param newlen The new length of the vector.  Resizes work either up or down - data is not guaranteed to be
 *               immutable if the resize is down, however.  Usually data gets cut off the end but that's
 *               not necessarily true.
 */
void resizeVector(vector_t *vec, size_t newlen);

/**
 * Copies a vector just in case that functionality is necessary, e.g. copying for modification, then comparing the two.
 *
 * @param vec Vector to copy
 * @return A new vector object that is the exact copy of vec parameter.
 */
vector_t *copyVector(vector_t *vec);

/**
 * Prints a vector to stdout
 *
 * @param vec Vector to print
 */
void printVector(vector_t *vec);

/**
 * Copies a slice of a vector_t object for whatever operation that is needed.
 *
 * @param vec The vector to be sliced
 * @param start Index of the start of the slice
 * @param end Index of the end of the slice (non-inclusive!)
 * @return A vector_t pointer to the slice of the original vector.  Does not affect the original.
 */
vector_t *sliceVector(vector_t *vec, size_t start, size_t end);

/**
 * Frees memory associated with a vector.
 *
 * @param vec Vector to delete
 */
void deleteVector(vector_t *vec);

/**
 * Takes the (strictly real) inner product of two vectors.
 *
 * @param vec1 Bra vector
 * @param vec2 Ket vector
 * @return Inner product of vec1 and vec2
 */
double innerProduct(const vector_t *vec1, const vector_t *vec2);

/**
 * Multiplies a vector by a scalar.
 *
 * @param vec Vector to be multiplied
 * @param scalar Scalar to multiply onto vector
 */
void scalarMultiply(vector_t *vec, const double scalar);

/**
 * Adds one vector to another component-wise.
 *
 * @param vec1 Vector that gets added to (this vector is modified)
 * @param vec2 Vector to add to vec1 (this vector is not modified)
 */
void addVector(vector_t *vec1, const vector_t *vec2);

void subVector(vector_t *vec1, const vector_t *vec2);

/**
 * Takes the norm of a vector.
 *
 * @param vec Vector to take the norm of
 * @return The squared norm of the vector (L-N norm I think)
 */
double normVector(const vector_t *vec);

vector_t *identity(vector_t *vec);

vector_t *radial(vector_t *vec);


#endif //HOMEAUTOMATION_VECTOROPS_H
