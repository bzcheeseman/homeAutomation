//
// Created by Aman LaChapelle on 8/27/16.
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

#ifndef HOMEAUTOMATION_HASHTABLE_H
#define HOMEAUTOMATION_HASHTABLE_H

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#include "../utilities.h"

/*
 * TODO: Fix documentation
 * TODO: Document structs
 * TODO: Add parameter that says how many map entries are full and a function to update that count
 */

typedef struct {
  char *key;
  char **neighbors;
  size_t num_neighbors;
} entry_t ;

typedef struct{
  entry_t *(*entries);
  size_t size;
  size_t window;
} map_t ;

/**
 * Constructs a map object with a given size.
 *
 * @param size Size of the map
 * @return pointer to a new map.
 */
void *newMap(size_t size, size_t n_grams);

/**
 * Frees memory allocated to the map/pointer.
 *
 * @param map The map to destruct
 */
void deleteMap(map_t* map);

/**
 * Hashes the key of an entry that will be added to the map.  Mostly internal use.
 *
 * @param map Map to which an entry will be added
 * @param key The key of the entry to be added
 * @return The hash for the entry
 */
long hashEntry(map_t *map, char *key);

/**
 * Adds an entry to the map.  External use for adding a key, value pair to the map.  Hashes the key, creates a new entry
 * and adds it to the map.
 *
 * Uses recursion to avoid collisions - if two keys hash to the same value but are not the same char *, the function
 * increments the bin by 1 and reruns the insertion.  This is repeated until an empty bin is found.
 *
 * @param map The map to which an entry will be added
 * @param key The key to this new entry
 * @param frequency The integer frequency of the bigram (key)
 * @param bin There for the recursive step - usually called with NULL in that parameter.
 */
void *addEntry(map_t *map, char *key, char **neighbors, long bin);

/**
 * Gets a specified entry from the key provided.
 *
 * @param map The map in which this entry (to be found) resides
 * @param key The key of the entry we want.
 * @param bin There for the recursive step - usually called with NULL in that parameter.
 * @return The int corresponding to the value of the entry corresponding to the key.
 */
void *getEntry(map_t *map, char *key, long bin);

/**
 * Deletes the map passed to it and returns a new map with all the same values contained in their new hashes in a new
 * map of size = newsize.
 *
 * @param map The map that needs resizing
 * @param newsize The new size of the map
 * @return A new map that has size = newsize.
 */
map_t *resizeMap(map_t *map, size_t newsize);


#endif //HOMEAUTOMATION_HASHTABLE_H
