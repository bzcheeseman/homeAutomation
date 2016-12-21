//
// Created by Aman LaChapelle on 12/20/16.
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


#include "../include/train.hpp"

corpus_t load_corpus(std::string filename, int N){

  std::ifstream corp_file (filename);

  corpus_t corpus;
  std::vector<std::string> raw_corpus;
  std::string buffer;
  std::string ss_temp;

  //extract each word - need to get rid of parenthesis, etc.
  while (std::getline(corp_file, buffer)){
    std::stringstream ss (buffer);
    while (ss >> ss_temp){
      for (int i = 0; i < ss_temp.size(); i++){
        if (ss_temp[i] == '"' || ss_temp[i] == ',' || ss_temp[i] == '.' || ss_temp[i] == '!' || ss_temp[i] == '?'){
          ss_temp.erase(ss_temp.begin() + i);
        }
      }
      raw_corpus.push_back(ss_temp);
      ss_temp.clear();
    }
    buffer.clear();
  }

  //now I need to find its N neighbors and put it all into a map
  int len = raw_corpus.size();
  std::vector<std::string> neighbors;

  for (int i = 0; i < len; i++){
    for (long j = -N; j <= N; j++){
      if (i+j > 0 && i+j < len && j != 0){
        neighbors.push_back(raw_corpus[i+j]);
      }
    }

    try {
      std::vector<std::string> &element = corpus.at(raw_corpus[i]); //nickname for the entry
      for (int i = 0; i < neighbors.size(); i++){
        element.push_back(neighbors[i]); //add the new neighbors to the entry. Duplicates are ok here because
                                         //that just means that context is common, which we need to know!
      }
    }
    catch (std::out_of_range) {
      corpus[raw_corpus[i]] = neighbors; //create a new neighbors list here
    }
  }

  //Now the corpus is built, so return it and process it later.

  return corpus;

}

void process_corpus(corpus_t corpus,
                    std::vector<matrix<unsigned int>> &one_hot,
                    std::vector<unsigned long> &labels){

  matrix<unsigned int> one (corpus.size(),1);

  one = uniform_matrix<unsigned int> (corpus.size(), 1, 0);

  //need to split up the corpus into little chunks, clearly

  for (auto iter = corpus.begin(); iter != corpus.end(); iter++){ //iterate over the map keys
    long index = std::distance(corpus.begin(), iter); //find the index of the word
    one(index,0) = 1;

    for (auto iter2 = (iter->second).begin(); iter2 != (iter->second).end(); iter2++){
      one_hot.push_back(one); //add the one-hot vector to the training input set

      auto word = corpus.find(*iter2); //need to find the place in corpus that the word sits at
      long word_index = std::distance(corpus.begin(), word);
      labels.push_back((unsigned long)word_index); //add the corresponding output to the training output set
    }
    one = uniform_matrix<unsigned int> (corpus.size(), 1, 0); //re-zero the dlib matrix
  }

}

std::ostream &operator<<(std::ostream &out, corpus_t &corpus) {

  for (auto iter = corpus.begin(); iter != corpus.end(); iter++){
    for (auto iter2 = (iter->second).begin(); iter2 != (iter->second).end(); iter2++){
      out << iter->first << ":\t" << *iter2 << "\n";
    }
  }

  return out;
}

std::istream &operator>>(std::istream &in, corpus_t &corpus){
  //key: value - read in just like that
  std::string buffer, ss_temp, key;

  while (std::getline(in, buffer)){
    std::stringstream ss (buffer);

    ss >> key;
    key.erase(key.end()-1, key.end());

    try {
      std::vector<std::string> &element = corpus.at(key); //nickname for the entry
      while (ss >> ss_temp){
        element.push_back(ss_temp); //add the new neighbors to the entry. Duplicates are ok here because
        //that just means that context is common, which we need to know!
        ss_temp.clear();
      }
    }
    catch (std::out_of_range) {
      std::vector<std::string> neighbors;
      while (ss >> ss_temp){
        neighbors.push_back(ss_temp); //add the new neighbors to the entry
        ss_temp.clear();
      }
      corpus[key] = neighbors;
    }

    key.clear();
    buffer.clear();
  }

  return in;

}

matrix<float> train_word2vec(word2vec network,
                             std::vector<matrix<unsigned int>> &one_hot,
                             std::vector<unsigned long> &labels) {

  dnn_trainer<word2vec> trainer(network);
  trainer.set_learning_rate(0.05);
  trainer.set_min_learning_rate(1e-5);
  trainer.set_max_num_epochs(1000);
  trainer.set_mini_batch_size(64);
  trainer.be_verbose();

  trainer.set_synchronization_file("../../logging/word2vec_sync", std::chrono::seconds(20));

  //doing it this way means that the trainer runs in a separate thread which should speed things up
  std::vector<matrix<unsigned int>> mini_batch_samples;
  std::vector<unsigned long> mini_batch_labels;
  dlib::rand rnd(time(0));
  while(trainer.get_learning_rate() >= trainer.get_min_learning_rate())
  {
    mini_batch_samples.clear();
    mini_batch_labels.clear();

    while(mini_batch_samples.size() < trainer.get_mini_batch_size())
    {
      auto idx = rnd.get_random_32bit_number()%one_hot.size();
      mini_batch_samples.push_back(one_hot[idx]);
      mini_batch_labels.push_back(labels[idx]);
    }

    trainer.train_one_step(mini_batch_samples, mini_batch_labels);
  }

  network.clean();
  serialize("../../logging/word2vec_model.dat") << network;

  //now I need to extract the learned hidden layer and return it
  dlib::resizable_tensor t = layer<3>(network).layer_details().get_weights();
  const long nr = t.nr();
  const long nc = t.nc();

  matrix<float> weights (nr, nc);
  for (int i = 0; i < nr; i++){
    for (int j = 0; j < nc; j++){
      weights(i,j) = t.host()[(nr + i)*nc + j];
    }
  }

  return weights; //return a matrix of the weights we found

}

//void load_resnet_images(std::string filename,
//                        std::vector<matrix<unsigned char>> training_images,
//                        std::vector<unsigned long> training_classes) {
//  ;
//}
//
//resnet_predict train_lenet(resnet_train network,
//                           std::vector<matrix<unsigned char>> training_images,
//                           std::vector<unsigned long> training_classes) {
//  ;
//}
