//
// Created by Aman LaChapelle on 1/21/17.
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


#ifndef HOMEAUTOMATION_SERVER_HPP
#define HOMEAUTOMATION_SERVER_HPP

#include <thread>
#include <vector>
#include <iostream>

#include <zmqpp/zmqpp.hpp>
#include <zmqpp/curve.hpp>

#include <dlib/serialize.h>

#include "dlib_zmq.hpp"

class worker {

  zmqpp::context &ctx_;
  zmqpp::socket worker_;

public:
  worker(zmqpp::context &ctx, int sock_type);

  void work();

};

class server {

  zmqpp::context ctx_;
  zmqpp::socket frontend_;
  zmqpp::socket backend_;

public:



};


#endif //HOMEAUTOMATION_SERVER_HPP
