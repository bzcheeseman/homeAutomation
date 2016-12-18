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


#include "../include/bigrams.h"

map_t *mapBigrams(char *phrase, map_t *bag){

  size_t end = strlen(phrase);

  int len = 1;
  char **wordlist = malloc(sizeof(char *));

  char last_char = phrase[0];
  int i, last_space;

  for (i = 0, last_space = 0; i < end; i++){

    if (phrase[i] == ' ' && last_char != ' '){
      wordlist[len-1] = calloc((size_t)(i-last_space) + 1, sizeof(char)); //malloc the new array

      strncpy(wordlist[len-1], phrase+last_space, i-last_space); //copy it over

      len++; //Change lengths and realloc things
      wordlist = realloc(wordlist, sizeof(char *) * len);

      last_space = i; //set the last space to the one we just found
    }

    last_char = phrase[i];

  }

  if (len < 2){
    return NULL;
  }

  wordlist[len-1] = malloc( sizeof(char) * ((end-last_space) + 1) ); //alloc the last array

  strncpy(wordlist[len-1], phrase+last_space, end-last_space); //copy it over

  for (int j = 1; j < len; j++){  //Removing the leading spaces from words
    wordlist[j] = wordlist[j]+1;
  }

  //Now we have all the words in the sentence split into an array - now we need to split them into the list of bigrams.

  len -= 1;

  if (!bag){
    bag = newMap(len<<1);
  }

  for (int ii = 0; ii < len; ii++){

    size_t bufsize = strlen(wordlist[ii]) + 1 + strlen(wordlist[ii+1]) + 1; //length1 + space + length2 + \0

    char *buf = malloc(sizeof(char) * bufsize);

    strncpy(buf, wordlist[ii], strlen(wordlist[ii]));
    strncat(buf+strlen(wordlist[ii]), " ", 1); //add trailing space to first word
    strncat(buf+strlen(wordlist[ii])+1, wordlist[ii+1], strlen(wordlist[ii+1]));

    addEntry(bag, buf, (int)NULL, (long)NULL);
  }

  return bag;
}

void printBag(map_t *bag) {
  printf("\n=============================\n");
  for (int i = 0; i < bag->size; i++){
    if (bag->entries[i]->key != NULL){
      printf("%s: %d\n", bag->entries[i]->key, bag->entries[i]->value);
    }
  }
  printf("=============================\n");
}

void addPhrase(char *phrase, map_t *bag) {

  bag = mapBigrams(phrase, bag);

  return;
}

void logtoFile(map_t *bag, char *filename) {
  if (filename == NULL){
    filename = "/Users/Aman/code/homeAutomation/logging/wordbag.log";
  }
  FILE *log = fopen(filename, "w+");
  int end = bag->size;
  fprintf(log, "%d\n", bag->size);
  fprintf(log, "# Bigram \t Frequency\n");
  for (int i = 0; i < end; i++){
    if (bag->entries[i]->key != NULL){
      fprintf(log, "%s\t%d\n", bag->entries[i]->key, bag->entries[i]->value);
    }
  }

  fclose(log);

  return;
}

map_t *importfromFile(char *filename) {
  if (filename == NULL){
    filename = "/Users/Aman/code/homeAutomation/logging/wordbag.log";
  }

  FILE *log = fopen(filename, "r+");

  char *num_lines = NULL;
  size_t len = 0;
  getline(&num_lines, &len, log);

  int length = atoi(num_lines);
  map_t *bag = newMap(length);

  int i = 0;
  char *buf = NULL;
  size_t l = 0;
  size_t read = 0;

  while ((read = getline(&buf, &l, log)) != -1){
    char *buf2;
    if (read <= 2 || buf[0] == '#'){
      ;
    }
    else{
      int j = 0;
      while (buf[j] != '\t'){
        j++;
      }
      buf2 = calloc(j, sizeof(char));
      strncpy(buf2, buf, j);
      addEntry(bag, buf2, atoi(buf+(j+1)), (long)NULL);
      i++;
    }
  }

  return bag;

}

void deleteBag(map_t *bag) {
  logtoFile(bag, NULL);
  deleteMap(bag);
  free(bag);
}