//
// Created by Aman LaChapelle on 8/27/16.
//

#include "../include/hashTable.h"

map_t *newMap(int size){

  if (!size){
    size = 32;
  }

  map_t *out = malloc(sizeof(map_t));
  if (!out){
    return NULL;
  }

  out->entries = calloc((size_t)size, sizeof(entry_t));
  if (!out->entries){
    return NULL;
  }

  for (int i = 0; i < size; i++){
    out->entries[i] = malloc(sizeof(entry_t *));
    out->entries[i]->key = NULL;
    out->entries[i]->value = 0;
  }

  out->size = size;

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

void addEntry(map_t *map, char *key, int frequency, long bin){
  if (!bin){
    bin = hashEntry(map, key);
  }

  if (map->entries[bin]->key == NULL){ //Nothing there
    if (frequency){
      map->entries[bin]->key = strdup(key);
      map->entries[bin]->value = frequency;
    }
    else{
      map->entries[bin]->key = strdup(key);
      map->entries[bin]->value++;
    }

  }
  else if (strcmp(map->entries[bin]->key, key) == 0){ //in the list already, increment frequency
    if (frequency){
      map->entries[bin]->value = frequency;
    }
    else{
      map->entries[bin]->value++;
    }
  }
  else if (strcmp(map->entries[bin]->key, key) != 0){ //2 different keys with the same hash
    bin++;
    bin %= map->size;
    addEntry(map, key, frequency, bin);
  }

  return;
}

int getEntry(map_t *map, char *key, long bin){
  if (!bin){
    bin = hashEntry(map, key);
  }

  if (strncmp(map->entries[bin]->key, key, strlen(key)) == 0){
    return map->entries[bin]->value;
  }
  else{
    bin++;
    bin %= map->size;
    getEntry(map, key, bin);
  }
}

map_t *resizeMap(map_t *map, int newsize) {
  map_t *newmap = newMap(newsize);

  for (int i = 0; i < map->size; i++){
    if (map->entries[i]->key != NULL){
      addEntry(newmap, map->entries[i]->key, map->entries[i]->value, (long)NULL);
    }
  }

  deleteMap(map);

  return newmap;
}

