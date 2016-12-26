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


#ifndef HOMEAUTOMATION_TRAIN_HPP
#define HOMEAUTOMATION_TRAIN_HPP

//dlib dependencies
#include <dlib/dnn.h>

//STL dependencies
#include <string>
#include <fstream>
#include <numeric>

#include "dataset.hpp"
#include "py_module.hpp"

#define PI 3.14159265358979323846264338327950288419716939937510

using namespace dlib;

template<typename Num>
inline std::ostream &operator<<(std::ostream &out, std::vector<Num> vec){
  std::for_each(vec.begin(), vec.end(), [&out](Num n){out << n << "\t";});
  return out;
}

class ocrnet{

//  static const unsigned long number_of_classes = 62; //0-9 numbers, capital A-Z and lowercase a-z.
  static const unsigned long number_of_classes = 10; //mnist 0-9 numbers

  //use different nets for letters vs numbers
  //work on dlib rnn

  //this works without rotations, make it work with rotations too

  //you want the first number (depth) in conv layers to come down to something like your final classes?

  //get closer to the actual inception net for more classes, or do I just do 0-9 on one net, A-Z on another and a-z on another?

  template <typename SUBNET> using stem = relu<con<16,3,3,1,1,relu<con<8,3,3,1,1,relu<con<4,3,3,2,2,SUBNET>>>>>>;

  template <typename SUBNET> using sub_block1 = relu<affine<con<10,1,1,1,1,SUBNET>>>;
  template <typename SUBNET> using sub_block1_t = relu<bn_con<con<10,1,1,1,1,SUBNET>>>;

  template <typename SUBNET> using sub_block2 = relu<affine<con<10,1,1,1,1,avg_pool<3,3,1,1,SUBNET>>>>;
  template <typename SUBNET> using sub_block2_t = relu<bn_con<con<10,1,1,1,1,avg_pool<3,3,1,1,SUBNET>>>>;

  template <typename SUBNET> using sub_block3 = relu<con<10,3,1,1,1,
                                                         relu<con<10,1,3,1,1,
                                                                  relu<con<10,3,1,1,1,
                                                                           relu<con<10,1,3,1,1,
                                                                                    relu<con<10,1,1,1,1,SUBNET
                                                                                    >>>>>>>>>>;

  template <typename SUBNET> using sub_block4 = relu<con<10,3,1,1,1,relu<con<10,1,3,1,1,relu<con<10,1,1,1,1,SUBNET>>>>>>;

  template <typename SUBNET> using ocrnetA = inception4<sub_block1,sub_block2,sub_block3,sub_block4,SUBNET>;
  template <typename SUBNET> using ocrnetA_t = inception4<sub_block1_t,sub_block2_t,sub_block3,sub_block4,SUBNET>;

  template <typename SUBNET> using con_31 = relu<con<10,3,1,1,1,SUBNET>>;
  template <typename SUBNET> using con_13 = relu<con<10,1,3,1,1,SUBNET>>;

  template <typename SUBNET> using fork = inception2<con_31,con_13,SUBNET>;

  template <typename SUBNET> using sub_block5 =  fork<relu<con<10,3,1,1,1,
                                                               relu<con<10,1,3,1,1,
                                                                        relu<con<10,1,1,1,1,SUBNET>>>>>>>;

  template <typename SUBNET> using sub_block6 = fork<relu<con<10,1,1,1,1,SUBNET>>>;

  template <typename SUBNET> using ocrnetB = inception3<sub_block1,sub_block5,sub_block6,SUBNET>;
  template <typename SUBNET> using ocrnetB_t = inception3<sub_block1_t,sub_block5,sub_block6,SUBNET>;



  using ocrnet_train = loss_multiclass_log<
          fc<number_of_classes,
             relu<fc<32, //had a max_pool layer right here, wondering if that helped or not...
                     ocrnetB_t<
                     max_pool<2,2,1,1,ocrnetA_t<
                                             stem<input<matrix<unsigned char>>>>>>>>>>;

  using ocrnet_predict = loss_multiclass_log<
          fc<number_of_classes,
             relu<fc<32,
                     ocrnetB<
                     max_pool<2,2,1,1,ocrnetA<
                                              stem<input<matrix<unsigned char>>>>>>>>>>;

  ocrnet_train training;
  ocrnet_predict predicting;

  std::string sync_folder;

  dataset fnt;
  dataset hnd;
  dataset mnist;

public:
  ocrnet(const std::string &sync_file);

  virtual ~ocrnet();

  void train(dataset::data_folder_t which, bool force_retrain = false);

};


#endif //HOMEAUTOMATION_TRAIN_HPP
