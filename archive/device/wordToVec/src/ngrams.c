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


#include "../include/ngrams.h"
#include "../../utilities/utilities.h"

void *mapNgrams(char *phrase, size_t N, map_t *bag){

  size_t end = strlen(phrase);

  size_t len = 1;
  char **wordlist = malloc(sizeof(char *));

  char last_char = phrase[0];
  int i, last_space = 0;

  for (i = 0; i < end; i++){
    if (phrase[i] == ' ' && last_char != ' '){
      wordlist[len-1] = malloc(((size_t)(i-last_space) + 1)*sizeof(char)); //malloc the new array

      strncpy(wordlist[len-1], phrase+last_space, i-last_space); //copy it over

      if (wordlist[len-1][0] == ' '){
        wordlist[len-1]++;
      }

      len++; //Change lengths and realloc things
      wordlist = realloc(wordlist, sizeof(char *) * len);

      last_space = i; //set the last space to the one we just found
    }
    last_char = phrase[i];
  } //now we have the phrase split into an array of words

  len--; //len ends up 1 too big

  if (len < 2*N){
    error_t *error = malloc(sizeof(error_t));
    error->what = "len of phrase is less than N, cannot split into N-grams!";
    return error;
  }

  if (!bag){
    bag = newMap(len<<1, N);
  }

  //We need to take each word and map it to its N nearest neighbors.
  for (size_t j = 0; j < len; j++){

    int N_forward_neighbor = j + N;
    int N_backward_neighbor = j - N;
    if (N_backward_neighbor < 0){
      N_backward_neighbor = 1;
    }
    if (N_forward_neighbor > len-1){
      N_forward_neighbor = len-2;
    }

    int num_words = N_forward_neighbor - N_backward_neighbor;

    char **neighbors = malloc((size_t)fmax((float)num_words, (float)1)*sizeof(char *));

    int l = 0;
    for (int k = N_backward_neighbor; k <= N_forward_neighbor; k++){
      if (k != j){
        neighbors[l] = strndup(wordlist[k], strlen(wordlist[k]));
        l++;
      }
      else{
        continue;
      }
    }
    addEntry(bag, wordlist[j], neighbors, l, NULL);
    free(neighbors);
  }

  return bag;
}

void printBag(map_t *bag) {
  printf("\n=============================\n");
  for (int i = 0; i < bag->size; i++){
    if (bag->entries[i]->key != NULL){
      int num_neighbors = bag->entries[i]->num_neighbors;
      for (int j = 0; j < num_neighbors; j++){
        printf("%s: %s\n", bag->entries[i]->key, bag->entries[i]->neighbors[j]);
      }
    }
  }
  printf("=============================\n");
}

void addPhrase(char *phrase, size_t window, map_t *bag) {

  bag = mapNgrams(phrase, window, bag);

  return;
}

void logtoFile(map_t *bag, char *filename) {
  if (filename == NULL){
    filename = "/Users/Aman/code/homeAutomation/logging/wordbag.data_log";
  }
  FILE *log = fopen(filename, "w+");
  int end = bag->size;
  fprintf(log, "%d # Bag size\n", bag->size);
  fprintf(log, "%d # Window\n", bag->window);
  fprintf(log, "# Word \t N-neighbors \n");
  for (int i = 0; i < end; i++){
    if (bag->entries[i]->key != NULL){
      for (int j = 0; j < bag->entries[i]->num_neighbors; j++){
        fprintf(log, "%s\t%s\n", bag->entries[i]->key, bag->entries[i]->neighbors[j]);
      }
    }
  }

  fclose(log);

  return;
}

//map_t *importfromFile(char *filename) {
//  if (filename == NULL){
//    filename = "/Users/Aman/code/homeAutomation/logging/wordbag.data_log";
//  }
//
//  FILE *data_log = fopen(filename, "r+");
//
//  char *num_lines = NULL;
//  size_t len = 0;
//  getline(&num_lines, &len, data_log);
//
//  int length = atoi(num_lines);
//  map_t *bag = newMap(length, 2);
//
//  int i = 0;
//  char *buf = NULL;
//  size_t l = 0;
//  size_t read = 0;
//
//  while ((read = getline(&buf, &l, data_log)) != -1){
//    char *buf2;
//    if (read <= 2 || buf[0] == '#'){
//      ;
//    }
//    else{
//      int j = 0;
//      while (buf[j] != '\t'){
//        j++;
//      }
//      buf2 = calloc(j, sizeof(char));
//      strncpy(buf2, buf, j);
//      addEntry(bag, buf2, atoi(buf+(j+1)), 2, (long)NULL);
//      i++;
//    }
//  }
//
//  return bag;
//
//}

void deleteBag(map_t *bag) {
  logtoFile(bag, NULL);
  deleteMap(bag);
  free(bag);
}