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

//svm_t *pegasos(vector_t **S, double *y, double lambda, double C, int T, size_t k, size_t lenS) {
//
//  svm_t *out = malloc(sizeof(svm_t));
//
//  size_t size = S[0]->len;
//  out->w = newVector(size, NULL);
//
//
//  vector_t *w = newVector(size, NULL);
//  for (int i = 0; i < size; i++){ //init w
//    w->data[i] = 1./(sqrt(lambda) * (double)size);
//  }
//
//  for (int t = 1; t <= T; t++){
//
//    vector_t *sum = newVector(size, NULL); //new sum vector for each iteration of t
//    vector_t **Aplus = calloc(lenS, sizeof(vector_t *)); //create the Aplus matrix
//
//    for (int j = 0; j < k; j++){
//
//      int chooseS = (int) (rand() % lenS); //choose a sample
//
//      if (y[chooseS]*(innerProduct(S[chooseS], w)) < 1.){ //if the inner product of the sample and w is less than one then
//                                                     //copy it over
//        Aplus[j] = newVector(S[chooseS]->len, S[chooseS]->data);
//
//        scalarMultiply(Aplus[j], y[chooseS]); //scale the copied vector
//        addVector(sum, Aplus[j]); //add it to the sum
//
//      }
//
//    }
//
//    double eta_t = 1./(lambda * (double)t); //calculate eta and w
//    scalarMultiply(w, (1.- eta_t * lambda));
//
//    scalarMultiply(sum, eta_t/k);
//
//    addVector(w, sum); //Already added the scaled previous w, now add the sum - this is w_{t+1/2}
//
//    if (1. < ((1./sqrt(lambda))/normVector(w)) ){ //if 1 < norm of scaled w_{t+1/2}, return w_{t+1/2} = w_{t+1}
//      ;
//    }
//    else if (1. > ((1./sqrt(lambda))/normVector(w))){ //if norm of w_{t+1/2} < 1 rescale w, return w_{t+1}
//      scalarMultiply(w, ( (1./sqrt(lambda))/normVector(w) ));
//    }
//
//
//    //clean up!
//    deleteVector(sum);
////    for (int i = 0; i < lenS; i++){
////      deleteVector(Aplus[i]);
////    }
//    free(Aplus);
//  }
//
//
//  out->kernelFunction = innerProduct;
//  out->w = w;
//
//  //return whichever w is the correct one, the rescaled one or the non-rescaled one.
//  return out;
//
//}

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

//svm_t *smoAlgorithm(double C, double tol, long max_passes, vector_t **data, double *y, long lenData,
//                    double (*kernelFunction)(const vector_t *, const vector_t *)) {
//
//  svm_t *out = malloc(sizeof(svm_t));
//
//  out->kernelFunction = kernelFunction; // stuff for the prediction function
//
//  out->alphas = newVector((size_t)lenData, NULL); //init alphas to zero
//  out->b = 0;
//
//  svm_t *old = malloc(sizeof(svm_t));
//  old->alphas = newVector((size_t)lenData, NULL); //old alphas
//  old->b = 0;
//
//  long passes = 0;
//
//  while (passes < max_passes){
//    long num_changed_alphas = 0;
//    for (int i = 0; i < lenData; i++){
//
//      double Ei; //Ei is exploding... something is wrong
//      double f_xi = 0;
//
//      for (int l = 0; l < lenData; l++){ //calculate f(xi)
//        f_xi += out->alphas->data[l] * y[l] * kernelFunction(data[l], data[i]) + out->b;
//      }
//
////      double f_xi = out->alphas->data[i] * y[i] * kernelFunction(data[i], data[i]) + out->b;
//
//      Ei = f_xi - y[i];
//
//      if ((y[i]*Ei < -tol && out->alphas->data[i] < C) || (y[i]*Ei > tol && out->alphas->data[i] > 0.0)){
//        long j = rand() % lenData;
//        if (j == i){
//          j = rand() % lenData;
//        }
//
//        double Ej;
//        double f_xj = 0;
//
//        //problem is in alphas?  They're bounded though...as is the kernel function...just out->b isn't
//
//        for (int m = 0; m < lenData; m++){ //calculate f(xi)
//          f_xj += out->alphas->data[m] * y[m] * kernelFunction(data[m], data[j]) + out->b;
//        }
//
////        double f_xj = out->alphas->data[j] * y[j] * kernelFunction(data[j], data[i]) + out->b;
//
//        Ej = f_xj - y[j];
//
//        old->alphas->data[i] = out->alphas->data[i];
//        old->alphas->data[j] = out->alphas->data[j];
//
//        double L, H;
//
//        if (y[i] == y[j]){
//          L = max(0.0, out->alphas->data[i] + out->alphas->data[j] - C);
//          H = min(C, out->alphas->data[i] + out->alphas->data[j]);
//        }
//        else{
//          L = max(0.0, out->alphas->data[j] - out->alphas->data[i]);
//          H = min(C, C + out->alphas->data[j] - out->alphas->data[i]);
//        }
//
//        if (L == H){
//          continue;
//        }
//
//        double eta = 2.*kernelFunction(data[i], data[j]) - kernelFunction(data[i], data[i]) - kernelFunction(data[j], data[j]);
//        if (eta >= 0){
//          continue;
//        }
//
//        out->alphas->data[j] -= y[j] * (Ei - Ej)/eta; //update alpha_j
//
//        if (out->alphas->data[j] > H){
//          out->alphas->data[j] = H;
//        }
//        else if (out->alphas->data[j] <= H && out->alphas->data[j] >= L){
//          ;
//        }
//        else if (out->alphas->data[j] < L){
//          out->alphas->data[j] = L;
//        }
//
//        //now alpha_j is clipped and updated
//
//        if (fabs( (out->alphas->data[j] - old->alphas->data[j]) ) < 1e-5){
//          continue;
//        }
//
//        out->alphas->data[i] += y[i] * y[j] * (old->alphas->data[j] - out->alphas->data[j]);
//
//        double b1 = out->b - //Ei -
//                y[i]*(out->alphas->data[i] - old->alphas->data[i])*kernelFunction(data[i], data[i])
//                    - y[j]*(out->alphas->data[j] - old->alphas->data[j])*kernelFunction(data[i], data[j]);
//        double b2 = out->b - //Ej -
//                    y[i]*(out->alphas->data[i] - old->alphas->data[i])*kernelFunction(data[i], data[j])
//                    - y[j]*(out->alphas->data[j] - old->alphas->data[j])*kernelFunction(data[j], data[j]);
//
//        if (0 < out->alphas->data[i] && out->alphas->data[i] < C){
//          out->b = b1;
//        }
//        else if (0 < out->alphas->data[j] && out->alphas->data[j] < C){
//          out->b = b2;
//        }
//        else{
//          out->b = 0.5 * (b1 + b2);
//        }
//
//        Ei = 0;
//        Ej = 0;
//        b1 = 0;
//        b2 = 0;
//
//        num_changed_alphas++;
//      }
//    }
//    if (num_changed_alphas == 0){
//      passes++;
//    }
//    else{
//      passes = 0;
//    }
//
//  }
//
//  out->w = newVector(2, NULL);
//  for (int i = 0; i < lenData; i++){
//    vector_t *copy = copyVector(data[i]);
//    scalarMultiply(copy, y[i]*out->alphas->data[i]);
//    addVector(out->w, copy);
//  }
//
//  return out;
//
//}

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




