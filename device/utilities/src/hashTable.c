//
// Created by Aman LaChapelle on 8/27/16.
//

#include "../include/hashTable.h"
#include "../utilities.h"

void *newMap(size_t size, size_t n_grams){

  if (!size){
    size = 10000;
  }

  map_t *out = malloc(sizeof(map_t));
  if (!out){
    error_t *error = malloc(sizeof(error_t));
    error->what = "Malloc failed for map";
    return error;
  }

  out->entries = malloc(size*sizeof(entry_t));
  if (!out->entries){
    error_t *error = malloc(sizeof(error_t));
    error->what = "Malloc failed for entries";
    return error;
  }

  out->entries = malloc(size*sizeof(entry_t *));

  for (int i = 0; i < size; i++){
    out->entries[i] = malloc(sizeof(entry_t *));
    out->entries[i]->key = NULL;
    out->entries[i]->neighbors = malloc(n_grams*sizeof(char *));
    out->entries[i]->num_neighbors = 0;
  }

  out->size = size;
  out->window = n_grams;

  return out;
}

void deleteMap(map_t *map){
  free(map->entries);
  free(map);
}

long hashEntry(map_t *map, char *key){
  unsigned long int hash = 0;
  int i = 0;
  while (hash < ULONG_MAX && i < strlen(key)){
    hash = hash<<4;
    hash += key[i];
    i++;
  }

  return hash % map->size;
}

void *addEntry(map_t *map, char *key, char **neighbors, size_t num_neighbors, long bin){
  if (!bin){
    bin = hashEntry(map, key);
  }

  error_t *error = malloc(sizeof(error_t));

  if (map->entries[bin]->key == NULL){ //Nothing there
    map->entries[bin]->key = strndup(key, strlen(key));
    for (int i = 0; i < 2*map->window; i++){
      if (neighbors[i] != NULL){
        map->entries[bin]->neighbors[i] = strndup(neighbors[i], strlen(neighbors[i]));
      }
      else{
        ;
      }
    }
    map->entries[bin]->num_neighbors = num_neighbors;
    error->what = NULL;
  }
  else if (strncmp(map->entries[bin]->key, key, strlen(key)) == 0){ //in the list already just extend the neighbors list
    size_t old_len = map->entries[bin]->num_neighbors;
    size_t new_len = old_len + num_neighbors;

    map->entries[bin]->neighbors = realloc(map->entries[bin]->neighbors, new_len);

    if (map->entries[bin]->neighbors){
      for (size_t i = old_len; i < new_len; i++){ //add the new neighbors to the list
        map->entries[bin]->neighbors[i] = strndup(neighbors[i-old_len], strlen(neighbors[i-old_len]));
      }
      error->what = NULL;
    }
    else{
      error->what = "Realloc failed in lengthening neighbors list";
    }
    map->entries[bin]->num_neighbors = new_len;
  }
  else if (strcmp(map->entries[bin]->key, key) != 0){ //2 different keys with the same hash
    bin++; //increment bin
    bin %= map->size; //mod it by the map size
    addEntry(map, key, neighbors, num_neighbors, bin); //try to add the entry again
  } //this will cause problems if the map is completely full, but that shouldn't be a problem

  return error;
}

void *getEntry(map_t *map, char *key, long bin){
  if (!bin){
    bin = hashEntry(map, key);
  }

  if (strncmp(map->entries[bin]->key, key, strlen(key)) == 0){
    return map->entries[bin]->neighbors;
  }
  else{
    bin++;
    bin %= map->size;
    getEntry(map, key, bin);
  }
}

map_t *resizeMap(map_t *map, size_t newsize) {
  map_t *newmap = (map_t *)newMap(newsize, map->window);

  for (int i = 0; i < map->size; i++){
    addEntry(newmap, map->entries[i]->key, map->entries[i]->neighbors, map->entries[i]->num_neighbors, (long)NULL);
  }

  deleteMap(map);

  return newmap;
}

