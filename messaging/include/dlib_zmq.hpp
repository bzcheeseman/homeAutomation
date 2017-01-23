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


#ifndef HOMEAUTOMATION_DLIB_TO_ZMQ_HPP
#define HOMEAUTOMATION_DLIB_TO_ZMQ_HPP

#include <dlib/serialize.h>
#include <zmqpp/zmqpp.hpp>

namespace dlib_zmq{

  template<typename serializable_type>
  void to_zmq(serializable_type &item, zmqpp::message &message){
    std::stringstream stream;
    dlib::serialize(item, stream);
    message << stream.str();
  }

  template<typename serializable_type>
  void from_zmq(serializable_type &item, zmqpp::message &message){
    std::string msg;
    message >> msg;
    std::stringstream stream(msg);
    dlib::deserialize(item, stream);
  }

} //dlib_zmq


#endif //HOMEAUTOMATION_DLIB_TO_ZMQ_HPP
