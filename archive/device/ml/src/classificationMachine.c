//
// Created by Aman LaChapelle on 8/20/16.
//

#include "../include/classificationMachine.h"

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




