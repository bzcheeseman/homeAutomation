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

#include "include/train.hpp"
#include <chrono>

int main(int argc, char *argv[]){

  word2vec net;
//  std::cout << net << std::endl;

  std::vector<matrix<unsigned int>> one_hot;
  std::vector<unsigned long> labels;

  corpus_t corpus;

  try{
    std::cout << "Trying to pull from existing data" << std::endl;
    deserialize("../../data/brown_processed/corpus.txt") >> corpus;
    deserialize("../../data/brown_processed/training_one_hot.txt") >> one_hot;
    deserialize("../../data/brown_processed/training_labels.txt") >> labels;
  }
  catch(...){
    std::cout << "No existing data, loading in new data" << std::endl;

    auto start = std::chrono::steady_clock::now();

    corpus = load_corpus("../../data/testing_data.txt", 1);

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start);
    std::cout << "Loading took: " << duration.count() << " sec" << std::endl;

    serialize("../../data/brown_processed/corpus.txt") << corpus;

    process_corpus(corpus, one_hot, labels);

    corpus.clear();

    serialize("../../data/brown_processed/training_one_hot.txt") << one_hot;
    serialize("../../data/brown_processed/training_labels.txt") << labels;
  }

  std::cout << corpus.size() << std::endl;

  try {
    matrix<float> weights = train_word2vec(net, one_hot, labels); //this is not working - the loss isn't updating right
    std::cout << weights << std::endl;
  }
  catch (std::exception &e){
    std::cout << e.what() << std::endl;
  }

  return 0;
}

