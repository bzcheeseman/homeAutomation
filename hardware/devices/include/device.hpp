//
// Created by Aman LaChapelle on 1/8/17.
//
// homeAutomation
// Copyright (c) 2017 Aman LaChapelle
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


#ifndef HOMEAUTOMATION_COMMUNICATE_HPP
#define HOMEAUTOMATION_COMMUNICATE_HPP

//dlib dependencies
#include <dlib/dnn.h>
#include <dlib/threads.h>
#include <dlib/misc_api.h>

//mitie dependencies
#include <mitie/conll_tokenizer.h>

//used for training the categorization model
#define MITIE_word_features "/usr/local/Cellar/mitie/0.4/share/MITIE-models/english/total_word_feature_extractor.dat"

//STL dependencies
#include <tuple>
#include <fstream>
#include <iomanip>


//Project dependencies
#include "../../../python/py_module.hpp"
#include "../../utilities/include/data_log.hpp"

template<class... Types>
class message_t {

private:
  const std::tuple<Types...> message;
  const size_t term;

public:
  message_t(const std::tuple<Types...> &message, const size_t &term) : message(message), term(term) {}
  message_t(std::tuple<Types...> &&message, size_t &&term) : message(message), term(term) {}

  inline const size_t getTerm() const { return term; }

};

//Tokenizes a file for training a text categorization model
inline std::vector<std::string> tokenize_file (const std::string& filename) {
  std::ifstream fin(filename.c_str());
  if (!fin)
  {
    std::cout << "Unable to load input text file" << std::endl;
    exit(EXIT_FAILURE);
  }
  // The conll_tokenizer splits the contents of an istream into a bunch of words and is
  // MITIE's default tokenization method.
  mitie::conll_tokenizer tok(fin);
  std::vector<std::string> tokens;
  std::string token;
  // Read the tokens out of the file one at a time and store into tokens.
  while(tok(token))
    tokens.push_back(token);

  return tokens;
}

class device {

private:
  const std::string name; //What we're calling the device, should be human-readable like 'Arduino board 1' or something
  const bool gpu_enabled; //Whether or not the device has a CUDA gpu on it.
  const size_t num_threads; //The number of threads a device CPU has.

  std::string unique_id; //IP address, MAC address, etc.
  size_t disk_available; //Amount of memory still available on the disk in kB
  size_t ram_available; //RAM available on the device in kB

  data_log my_log;

  pycpp::py_module *slack;

  const dlib::thread_pool pool;

public:
  device(const std::string &name, const std::string &unique_id, const bool gpu_enabled, const size_t num_threads);

  virtual ~device();

  const std::string &getName() const;

  const size_t getNum_threads() const;

  const bool isGpu_enabled() const;

  const std::string &getUnique_id() const;

  size_t getDisk_available() const;

  size_t getRam_available() const;

  void run_slack_server();

  void share_logs(device &other);

};


#endif //HOMEAUTOMATION_COMMUNICATE_HPP