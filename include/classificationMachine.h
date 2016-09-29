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
#include "vectorOps.h"
#include "hashTable.h"

/*
 * TODO: Fix this to classify bigrams somehow
 */

/**
 * @file classificationMachine.h
 * @brief The actual classifier and associated helper functions
 *
 * Holds a simple SVM/Naive Bayes implementation and all the associated helper functions.  Splits a given phrase
 * into bigrams and calculates bigram frequency in the phrase.  This will allow us to make use of the SVM/Naive Bayes
 * implementation to classify bigrams and determine which function to execute based on the word classification.
 *
 * Will also likely need a decision tree model or something in order to translate command bigrams to functions.
 */

/**
 *
 * @param phrase The phrase to be split into bigrams
 * @param bag Usually called with NULL here - used for the addPhrase(char*,map_t*) function.
 * @return A new (or modifies the old one if it's there) word bag that contains the bigrams of the inputted phrase.
 */
map_t *mapBigrams(char *phrase, map_t *bag);

/**
 * Adds a phrase to the word bag - will come in handy if we're reading from a file, for example.  Also means we can
 * build up a nice big word bag.
 *
 * Basically a wrapper for mapBigrams(char*,map_t*)
 *
 * @param phrase Phrase to be added to the word bag
 * @param bag The bag to which we add the phrase
 */
void addPhrase(char *phrase, map_t *bag);

/**
 * Prints everything about a wordbag_t so that the results can be verified by a human if required.
 *
 * @param bag map_t to be printed
 */
void printBag(map_t *bag);

/**
 * Logs the wordbag to a file - to build up a training dataset if we need one, quickly.
 * Writes the first character of the file to be the number of entries.
 *
 * @param bag Bag to log to file
 * @param filename Name of the file, defaults to logging/wordbag.log
 */
void logtoFile(map_t *bag, char *filename);

/**
 * Imports a word bag from a saved file - creates a new one.  ONLY WORKS ON BAGS WRITTEN WITH logtoFile(wordbag_t*, char*)
 *
 * @param filename The name of the file, defaults to logging/wordbag.log
 * @return A new wordbag_t object to be used later.
 */
map_t *importfromFile(char *filename);

void deleteBag(map_t *bag);




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
  vector_t *alphas;
  vector_t *w;
  double b;
  double (*kernelFunction)(const vector_t *, const vector_t *);
} svm_t ;

/**
 * Basic implementation of pegasos algorithm from:
 *              Pegasos: Primal Estimated sub-GrAdient SOlver for SVM by Shalev-Shwartz, et. al.
 *
 * Literally copied the algorithm directly.
 *
 * Maybe it works, highly dependent on the range of the data?  It's a little odd maybe?  Also added the C parameter
 *
 * @param S The input points (a list of vectors x)
 * @param y The correct outputs for training (a list of numbers y)
 * @param lambda Roughly the inverse of the learning rate
 * @param C A scaling factor that allows for data that's of limited range.  Around 5-10 works well here it seems like.
 * @param T The number of iterations to perform
 * @param k The length of subsets to be used
 * @param lenS The length of the input points matrix (how many points there are)
 */
svm_t *pegasos(vector_t **S, double *y, double lambda, double C, int T, size_t k, size_t lenS);

double max(double first, double second);
double min(double first, double second);

/**
 * b is blowing up for some reason, I dunno wtf is happening - need to step through (with debugger?)
 * removed the subtracting of Ei/Ej - this might be bad, but it seems to be working and I can't figure out how else to fix it
 * english brown corpus
 *
 * @param C The margin of error - soft margin so the softness of the margin.  Too small and it goes haywire
 * @param tol The tolerance of the thing - set to something small ~ 1e-4
 * @param max_passes Max passes to take in optimizing
 * @param data Training data
 * @param y Expected outcomes from training data
 * @param lenData Length of the training dataset
 * @param kernelFunction Callback to the kernel function - testing with a simple inner product
 * @return svm_t that holds the kernel function and other relevant data.
 */
svm_t *smoAlgorithm(double C, double tol, long max_passes, vector_t **data, double *y, long lenData,
                    double (*kernelFunction)(const vector_t *, const vector_t *));

/**
 * Implements the coordinate descent algorithm.  Really like this one - seems to work really really well.
 * Still need to test some kernel functions and stuff, but I'm happy overall for now.  Maybe varargs for the kernel function?
 * Need some sort of signature for that - needs like the gamma factor for the gaussian kernel...
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
                    double (*kernelFunction)(const vector_t *, const vector_t *),
                    vector_t *transformFunction(const vector_t *));

int prediction(svm_t *params, vector_t *testVector);


/*
 * SVM - Implementation TBD
 *
 * -use laplacian matrix to generate coord. space of words?  Or just simple bag of words?
 */

#endif //HOMEAUTOMATION_CLASSIFICATIONMACHINE_H
