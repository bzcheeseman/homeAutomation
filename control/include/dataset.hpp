//
// Created by Aman LaChapelle on 12/23/16.
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


#ifndef HOMEAUTOMATION_DATASET_HPP
#define HOMEAUTOMATION_DATASET_HPP

//dlib dependencies
#include <dlib/data_io.h>
#include <dlib/image_io.h>
#include <dlib/image_transforms.h>
#include <dlib/array2d.h>
#include <dlib/threads.h>
#include <dlib/misc_api.h>
#include <dlib/gui_widgets.h>

//STL dependencies
#include <string>
#include <fstream>
#include <cassert>

using namespace dlib;

inline void invert_image(matrix<unsigned char> &img){
  for (int i = 0; i < img.nr(); i++){
    for (int j = 0; j < img.nc(); j++){
      img(i,j) = (unsigned char)abs(img(i,j) - 255);

    }
  }
}

class dataset {

  const size_t num_classes = 62;

  size_t samples_per_label;
  size_t test_samples_per_label;

  struct hnd_t {
    std::string folder = "/Hnd/Img";
    std::string img_prefix_under10 = "-00";
    std::string img_prefix_under100 = "-0";
    std::string ID = "hnd";
  } Hnd ;

  struct fnt_t {
    std::string folder = "/Fnt";
    std::string img_prefix_under10 = "-0000";
    std::string img_prefix_under100 = "-000";
    std::string ID = "fnt";
  } Fnt ;

  struct mnist_t {
    std::string folder = "/mnist_train";
    std::string ID = "mnist";
  } Mnist ;

  const std::string folder;

  std::vector<matrix<unsigned char>> training_images;
  std::vector<unsigned long> training_labels;
  std::vector<matrix<unsigned char>> testing_images;
  std::vector<unsigned long> testing_labels;

  thread_pool tp;

private:
  void load();

public:
  static const int image_dim = 2<<4; //The size of the mnist images is 20

  enum data_folder_t {HAND, FONT, MNIST};

  dataset(std::string folder, data_folder_t which); //need to add in something in case the files are already there
  ~dataset();

  void operator()(const size_t index, matrix<unsigned char> &image, unsigned long &label, const bool test = false);
  void operator()(size_t index1, size_t index2,
             std::vector<matrix<unsigned char>> &images,
             std::vector<unsigned long> &labels, bool test = false);
  size_t size_train(){ return training_labels.size(); }
  size_t size_test(){ return testing_labels.size(); }

  bool ready(){ return size_train() > 0; }

private:
  const data_folder_t which;

  void _get_sample(size_t index, matrix<unsigned char> &image, unsigned long &label, bool test);

};


#endif //HOMEAUTOMATION_DATASET_HPP
