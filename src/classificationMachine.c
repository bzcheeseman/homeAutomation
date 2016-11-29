//
// Created by Aman LaChapelle on 8/20/16.
//

#include "../include/classificationMachine.h"
#include "../include/vectorOps.h"

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
  ssize_t read = 0;

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

double max(double first, double second) {
  if (first > second){
    return first;
  }
  else{
    return second;
  }
}

double min(double first, double second) {
  if (first < second){
    return first;
  }
  else{
    return second;
  }
}

double gaussian(vector_t *vec, const vector_t *other, kernelParams_t params) {
  subVector(vec, other);
  return exp(-params.gamma*pow(normVector(vec), 2));
}

double poly(const vector_t *vec, const vector_t *other, kernelParams_t params) {
  return pow(params.r + params.gamma*innerProduct(vec, other), params.d);
}

int prediction(svm_t *svm, vector_t *testVector) {
  double fx = svm->kernelFunction(testVector, svm->w, svm->params) + svm->b;
  if (fx > 1e-10){
    return 1;
  }
  else if (fx < -1e-10){
    return -1;
  }
  else{
    return 0;
  }
}

svm_t *coordDescent(vector_t **data, double *y, long lenData, double tol, double C, kernelFunc kernel, kernelParams_t params) {

  svm_t *out = malloc(sizeof(svm_t));

  size_t w_dim = data[0]->len + 1;

  out->alphas = newVector(lenData, NULL);
  out->kernelFunction = kernel;
  out->w = newVector(w_dim, NULL);
  out->params = params;

  int done = 0;
  int passes = 0;
  int max_passes = 10000;

  while (done == 0 && passes < max_passes){
    for (int i = 0; i < lenData; i++){
      vector_t *data_i = newVector(w_dim, data[i]->data);
      data_i->data[w_dim-1] = 1.0;

      //Lagrange multiplier update rule
      double g = y[i]*kernel(data_i, out->w, params) - 1.;
      double Qii = y[i]*y[i]*kernel(data_i, data_i, params);
      double alphaprime = min( max( out->alphas->data[i] - g/Qii, 0 ), C );

      //Update w
      scalarMultiply(data_i, y[i]*(alphaprime - out->alphas->data[i]));
      addVector(out->w, data_i);

      //update multiplier
      out->alphas->data[i] = alphaprime;
    }

    for (int i = 0; i < lenData; i++){
      if (fabs(out->alphas->data[i]) <= C){
        done = 1;
      }
      else{
        done = 0;
      }
    }
    passes++;
  }

  double outdata[w_dim-1];
  for (int i = 0; i < w_dim; i++){
    outdata[i] = out->w->data[i];
  }
  out->b = out->w->data[w_dim];
  out->w = newVector(w_dim-1, outdata);

  return out;
}

svm_t *conjugateGradient(vector_t **data, double *y, long lendata, double tol, double C, kernelFunc kernel, kernelParams_t params){
  
  svm_t *out = malloc(sizeof(svm_t));
  
  //need to look at conjugate gradient from other code - might work really well here.
  //need to think carefully about how to do it here - do I want a kernel or should I just pass
  //in a vector that's already been multiplied like in the project?
}




