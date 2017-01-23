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

#include "include/ocrnet.hpp"

int main(int argc, char *argv[]){

  try {

//    ocrnet net_hnd ("../../logging/resnet_logs");

    //So font and mnist datasets work great (or pretty well at least...)
    ocrnet net_fnt ("../../logging/resnet_logs");
//    ocrnet net_mnist("../../logging/resnet_logs");

//    std::cout << net_hnd.get_size() << std::endl;

//    net_hnd.train(dataset::HAND, true);
    net_fnt.train(dataset::FONT);
//    net_mnist.train(dataset::MNIST);

//    std::cout << net_hnd.get_size() << std::endl;

  }
  catch (std::exception &e){
    std::cout << e.what() << std::endl;
  }

  return 0;
}

