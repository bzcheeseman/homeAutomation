//
// Created by Aman LaChapelle on 1/10/17.
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


#ifndef HOMEAUTOMATION_CUDNN_RNN_H_HPP
#define HOMEAUTOMATION_CUDNN_RNN_H_HPP

//dlib dependences
#include <dlib/dnn/tensor.h>
#include <dlib/dnn.h>
#include <dlib/dnn/cuda_errors.h>

//cudnn dependencies
#include <cudnn.h>

//RNNMode_t = CUDNN_LSTM or CUDNN_GRU or CUDNN_RNN_RELU or CUDNN_RNN_TANH
//direction mode = CUDNN_UNIDIRECTIONAL (front to end only) or CUDNN_BIDIRECTIONAL (separatly front to back and back to front)
//input mode = CUDNN_LINEAR_INPUT (matrix mult) or CUDNN_SKIP_INPUT (input needs to be the right size for the first layer)
//dropout descriptor - create with cudnnCreateDropoutDescriptor() and cudnnSetDropoutDescriptor()

enum rnn_mode {RNN_RELU, RNN_TANH, LSTM, GRU};
enum direction_mode {UNIDIRECTIONAL, BIDIRECTIONAL};
enum input_mode {LINEAR, SKIP};

//Assumes that we are going to use CUDNN
template<rnn_mode rnn_mode,
        direction_mode rnn_direction,
        input_mode rnn_input_mode,
        int rnn_hidden_size, //number of layers across (at one time)
        int rnn_num_layers   //number of layers deep (at any time)
>
class rnn_ {

private:
  tensor h_in, h_out;
  tensor c_in, c_out;
  tensor w_in, w_out;
  tensor input;
  tensor output;

  //workspace from CUDNN api calls

public:
  rnn_(){}
  rnn_(const rnn_ &other){
    ;
  }

  template<typename SUBNET>
  void setup(const SUBNET &sub){
    ;
  }

  template <typename SUBNET>
  void forward(const SUBNET& sub, resizable_tensor& data_output){
    ;
  }

  template <typename SUBNET>
  void backward(
          const tensor& computed_output, // this parameter is optional
          const tensor& gradient_input,
          SUBNET& sub,
          tensor& params_grad){
    ;
  }

  void forward_inplace(const tensor& data_input, tensor& data_output){
    ;
  }

  void backward_inplace(
          const tensor& computed_output, // this parameter is optional
          const tensor& gradient_input,
          tensor& data_grad,
          tensor& params_grad){
    ;
  }

  const tensor& get_layer_params() const {
    ;
  }

  tensor& get_layer_params(){
    ;
  }

};

std::ostream& operator<<(std::ostream& out, const rnn_& item){
  ;
}
/*!
    print a string describing this layer.
!*/

void serialize(const rnn_& item, std::ostream& out);
void deserialize(rnn_& item, std::istream& in);
/*!
    provides serialization support
!*/


template <typename SUBNET>
using rnn = add_layer<rnn_, SUBNET>;

#endif //HOMEAUTOMATION_CUDNN_RNN_H_HPP
