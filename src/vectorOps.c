//
// Created by Aman LaChapelle on 8/21/16.
//

#include "../include/vectorOps.h"

vector_t *newVector(size_t len, double *data){

  vector_t *vec = malloc(sizeof(vector_t));

  vec->data = calloc(len, sizeof(double));
  vec->len = len;

  if (data){
    memcpy(vec->data, data, sizeof(double) * len);
  }

  return vec;

}

void resizeVector(vector_t *vec, size_t newlen){

  if (newlen >= vec->len){
    vec->data = realloc(vec->data, sizeof(double) * newlen);
    vec->len = newlen;
  }
  else{
    double *newData = calloc(newlen, sizeof(double));
    memcpy(newData, vec->data, sizeof(double) * newlen);

    //release the old memory for vec->data
    free(vec->data);

    //re-init vec->data
    vec->data = calloc(newlen, sizeof(double));

    //copy everything back
    memcpy(vec->data, newData, sizeof(double) * newlen);
    vec->len = newlen;

    free(newData);
  }

}

vector_t *copyVector(vector_t *vec){

  vector_t *newVec = newVector(vec->len, vec->data);

  return newVec;
}

void printVector(vector_t *vec){

  size_t end = vec->len;
  for (size_t i = 0; i < end; i++){
    printf("%lf ", vec->data[i]);
  }
  printf("\n");

}

vector_t *sliceVector(vector_t *vec, size_t start, size_t end){

  assert(end <= vec->len);
  vector_t *slice = newVector(end-start, NULL);

  for (size_t i = start; i < end; i++){
    slice->data[i-start] = vec->data[i];
  }

  return slice;

}

void deleteVector(vector_t *vec) {
  free(vec->data);
  free(vec);
}

double innerProduct(const vector_t *vec1, const vector_t *vec2) {
  assert(vec1->len == vec2->len);
  int end = (int)vec1->len;

  double out = 0.0;

  for (int i = 0; i < end; i++){
    out += vec1->data[i] * vec2->data[i];
  }

  return out;
}

void scalarMultiply(vector_t *vec, const double scalar) {
  int end = vec->len;
  for (int i = 0; i < end; i++){
    vec->data[i] *= scalar;
  }

  return;
}

void addVector(vector_t *vec1, const vector_t *vec2) {

  assert(vec1->len == vec2->len);
  int end = vec1->len;

  for (int i = 0; i < end; i++){
    vec1->data[i] += vec2->data[i];
  }

  return;

}

void subVector(vector_t *vec1, const vector_t *vec2) {

  assert(vec1->len == vec2->len);
  int end = vec1->len;

  for (int i = 0; i < end; i++){
    vec1->data[i] -= vec2->data[i];
  }

  return;

}

double normVector(const vector_t *vec) {
  int end = vec->len;
  double out = 0;
  for (int i = 0; i < end; i++){
    out += pow(vec->data[i], 2);
  }

  out = sqrt(out);

  return out;
}

vector_t *identity(const vector_t *vec) {
  return vec;
}


