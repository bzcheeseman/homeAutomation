//
// Created by Aman LaChapelle on 8/20/16.
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

#ifndef HOMEAUTOMATION_CLASSIFICATIONMACHINE_H
#define HOMEAUTOMATION_CLASSIFICATIONMACHINE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

//Homemade vector operations - tried and tested
#include "../../utilities/include/vector.h"

/*
 * TODO: Fix this to classify bigrams somehow
 * TODO: Apply conjugate gradient to SVM?
 */

/*
 * SVM is a test implementation, will be designed around manual input, then once it definitely works will
 * make it work with the word frequencies calculated by the word bag methods.
 *
 * Maybe just switch to using FANN?  Or even just a different ML algorithm?
 *
 * Trying to accomplish:
 *  - take natural language commands
 *  - take a command and use functions provided to actually do the things in the commands
 *
 * How to take a command and break it down into actions:
 *  - break down the phrase
 *  - trigger bigrams?  maybe a neural network to make the decisions?
 */

/**
 * @struct svm_t
 * @brief Holds callback pointers to SVM functions and other data structures (if we need any?)
 *
 * Basic SVM struct that will hold everything needed for the SVM operations.  Pretty fucked up rn.
 */
//typedef struct {
//  double (*classifier)(double **points);
//  double (*trainer)(double **points, double *classes);
//
//  vector_t *w;
//  vector_t *b;
//
//} svm_t ;

typedef struct {
  double gamma;
  double r;
  double d;
} kernelParams_t;

typedef double(*kernelFunc)(const vector_t *, const vector_t *, kernelParams_t);
typedef vector_t*(*transformFunc)(const vector_t *);

typedef struct {
  vector_t *alphas;
  vector_t *w;
  double b;
  kernelFunc kernelFunction;
  kernelParams_t params;
} svm_t ;

double poly(const vector_t *vec, const vector_t *other, kernelParams_t params);

double gaussian(vector_t *vec, const vector_t *other, kernelParams_t params);

/**
 * Implements the coordinate descent algorithm.  Really like this one - seems to work really really well.
 * Still need to test some kernel functions and stuff, but I'm happy overall for now.  Maybe varargs for the kernel function?
 * Need some sort of signature for that - needs like the gamma factor for the gaussian kernel...
 * 
 * change to a matrix-free method for the kernel/multiplications?  That could be real useful...
 *
 * check out libsvm - look at "svm.h" and readme in /usr/local/Cellar/libsvm/3.21/README
 *
 * @param data
 * @param y
 * @param lenData
 * @param tol
 * @param C
 * @param kernelFunction
 * @return
 */
svm_t *coordDescent(vector_t **data, double *y, long lenData, double tol, double C,
                    kernelFunc kernel, kernelParams_t params);

int prediction(svm_t *params, vector_t *testVector);

//Need a word2vec thing


/*
 * SVM - Implementation TBD
 *
 * -use laplacian matrix to generate coord. space of words?  Or just simple bag of words?
 */

#endif //HOMEAUTOMATION_CLASSIFICATIONMACHINE_H
