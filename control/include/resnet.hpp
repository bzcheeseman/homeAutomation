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

using namespace dlib;

template<typename Num>
inline std::ostream &operator<<(std::ostream &out, std::vector<Num> vec){
  std::for_each(vec.begin(), vec.end(), [&out](Num n){out << n << "\t";});
  return out;
}

class resnet{

  template <
          int N,
          template <typename> class BN,
          int stride, typename SUBNET
  >
  using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

  template <
          template <int,template<typename>class,int,typename> class block,
          int N,
          template<typename>class BN, typename SUBNET
  >
  using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

  template <
          template <int,template<typename>class,int,typename> class block,
          int N,
          template<typename>class BN, typename SUBNET
  >
  using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

  template <typename SUBNET> using res       = relu<residual<block,8,bn_con,SUBNET>>;
  template <typename SUBNET> using ares      = relu<residual<block,8,affine,SUBNET>>;
  template <typename SUBNET> using res_down  = relu<residual_down<block,8,bn_con,SUBNET>>;
  template <typename SUBNET> using ares_down = relu<residual_down<block,8,affine,SUBNET>>;

  static const unsigned long number_of_classes = 62; //0-9 numbers, capital A-Z and lowercase a-z.
  using resnet_train = loss_multiclass_log<fc<number_of_classes,
                                              avg_pool_everything<
                                                      res<res<res<res_down<repeat<9,res, // repeat this layer 9 times
                                                                                  res_down<res<input<matrix<unsigned char>>
                                                                                  >>>>>>>>>>;


  template <typename SUBNET>
  using pres  = prelu<add_prev1<bn_con<con<8,3,3,1,1,prelu<bn_con<con<8,3,3,1,1,tag1<SUBNET>>>>>>>>;

  using resnet_predict = loss_multiclass_log<fc<number_of_classes,
                                                avg_pool_everything<
                                                        ares<ares<ares<ares_down<
                                                                repeat<9,ares,
                                                                       ares_down<ares<input<matrix<unsigned char>>
                                                                       >>>>>>>>>>;

  resnet_train training;
  resnet_predict predicting;

  std::string sync_folder;

  dataset fnt;
  dataset hnd;

public:
  resnet(const std::string &sync_file);

  virtual ~resnet();

  void train(dataset::data_folder_t which, bool force_retrain = false);

};


#endif //HOMEAUTOMATION_TRAIN_HPP
