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


#include "../include/dataset.hpp"

dataset::dataset(std::string folder, data_folder_t which)
        : folder(folder), which(which), tp(4) {

  std::string ID;

  switch (which) {
    case HAND :
      ID = Hnd.ID;
      samples_per_label = 40;
      test_samples_per_label = 15;
      break;
    case FONT :
      ID = Fnt.ID;
      samples_per_label = 60;
      test_samples_per_label = 15;
      break;
  }

  try {
    deserialize(folder+"/processed/training_images_"+ID) >> this->training_images;
    deserialize(folder+"/processed/training_labels_"+ID) >> this->training_labels;
    deserialize(folder+"/processed/testing_images_"+ID) >> this->testing_images;
    deserialize(folder+"/processed/testing_labels_"+ID) >> this->testing_labels;
  }
  catch(...){
    tp.add_task(*this, &dataset::load);
  }

}


dataset::~dataset() {
  tp.wait_for_all_tasks();

  std::string ID;

  switch (which) {
    case HAND :
      ID = Hnd.ID;
      break;
    case FONT :
      ID = Fnt.ID;
      break;
  }

  serialize(this->folder + "/processed/training_images_"+ID) << this->training_images;
  serialize(this->folder + "/processed/training_labels_"+ID) << this->training_labels;
  serialize(this->folder + "/processed/testing_images_"+ID) << this->testing_images;
  serialize(this->folder + "/processed/testing_labels_"+ID) << this->testing_labels;

}

void dataset::load(){
  try {

    std::vector<matrix<unsigned char>> training_images;
    std::vector<unsigned long> training_labels;
    std::vector<matrix<unsigned char>> testing_images;
    std::vector<unsigned long> testing_labels;

    training_images.reserve(num_classes*samples_per_label);
    training_labels.reserve(num_classes*samples_per_label);
    testing_images.reserve(num_classes*test_samples_per_label);
    testing_labels.reserve(num_classes*test_samples_per_label);

    std::string folder_name, prefix_under10, prefix_under100;

    switch (which) {
      case HAND :
        folder_name = Hnd.folder;
        prefix_under10 = Hnd.img_prefix_under10;
        prefix_under100 = Hnd.img_prefix_under100;
        break;
      case FONT :
        folder_name = Fnt.folder;
        prefix_under10 = Fnt.img_prefix_under10;
        prefix_under100 = Fnt.img_prefix_under100;
        break;
    }

    for (int i = 1; i < num_classes+1; i++) {
      std::string sample_no;

      if (i < 10) {
        sample_no = this->folder + folder_name + "/Sample00" + std::to_string(i) + "/img00" + std::to_string(i);
      } else {
        sample_no = this->folder + folder_name + "/Sample0" + std::to_string(i) + "/img0" + std::to_string(i);
      }

      array2d<unsigned char> img;
      array2d<unsigned char> scaled_img(image_dim, image_dim);

      for (int j = 1; j <= samples_per_label; j++) {

        if (j < 10) {
          load_image(img, sample_no + prefix_under10 + std::to_string(j) + ".png");
        } else {
          load_image(img, sample_no + prefix_under100 + std::to_string(j) + ".png");
        }

        resize_image(img, scaled_img);

        training_images.push_back((matrix<unsigned char>)mat(scaled_img));
        training_labels.push_back((unsigned long) i - 1);
      }

      for (int l = samples_per_label+1; l <= samples_per_label + test_samples_per_label; l++) {

        load_image(img, sample_no + prefix_under100 + std::to_string(l) + ".png");

        resize_image(img, scaled_img);

        testing_images.push_back((matrix<unsigned char>)mat(scaled_img));
        testing_labels.push_back((unsigned long) i - 1);
      }

    }

    this->training_images = training_images;
    training_images.clear();
    this->training_labels = training_labels;
    training_labels.clear();
    this->testing_images = testing_images;
    testing_images.clear();
    this->testing_labels = testing_labels;
    testing_labels.clear();

    this->training_images.shrink_to_fit();
    this->testing_images.shrink_to_fit();
    this->training_labels.shrink_to_fit();
    this->testing_labels.shrink_to_fit();

  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void dataset::operator()(const size_t index, matrix<unsigned char> &image, unsigned long &label, const bool test) {
  dlib::future<matrix<unsigned char>> img; dlib::future<unsigned long> lab;
  dlib::future<size_t> idx = index; dlib::future<bool> tst = test;

  tp.add_task(*this, &dataset::_get_sample, idx, img, lab, tst);

  image = img.get();
  label = lab.get();
}

void dataset::operator()(size_t index1, size_t index2,
                         std::vector<matrix<unsigned char>> &images,
                         std::vector<unsigned long> &labels,
                         bool test) {

  assert(index1 < index2);

  try {

    matrix<unsigned char> temp_img;
    unsigned long temp_label;

    for (size_t i = index1; i < index2; i++) {
      this->operator()(i, temp_img, temp_label, test);
      images.push_back(temp_img);
      labels.push_back(temp_label);
    }

  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }

}

void dataset::_get_sample(size_t index, matrix<unsigned char> &image, unsigned long &label, bool test) {
  try {

    if (test) {

      image = testing_images[index];
      label = testing_labels[index];

    } else {

      image = training_images[index];
      label = training_labels[index];

    }

  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}
