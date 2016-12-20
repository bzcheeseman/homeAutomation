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


#ifndef HOMEAUTOMATION_BIGRAMS_H
#define HOMEAUTOMATION_BIGRAMS_H

#include "../../utilities/include/hashTable.h"
#include "../../utilities/utilities.h"

/*
 * TODO: implement context extraction instead of bigrams - will allow to train the word2vec model.
 * TODO: Need to completely redo the way I map the n-grams
 */

/**
 * @file ngrams.h
 * @brief N-grams mapping functions for extracting context
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
void *mapNgrams(char *phrase, size_t N, map_t *bag);

/**
 * Adds a phrase to the word bag - will come in handy if we're reading from a file, for example.  Also means we can
 * build up a nice big word bag.
 *
 * Basically a wrapper for mapBigrams(char*,map_t*)
 *
 * @param phrase Phrase to be added to the word bag
 * @param window Context window
 * @param bag The bag to which we add the phrase
 */
void addPhrase(char *phrase, size_t window, map_t *bag);

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
//map_t *importfromFile(char *filename);

void deleteBag(map_t *bag);

#endif //HOMEAUTOMATION_BIGRAMS_H
