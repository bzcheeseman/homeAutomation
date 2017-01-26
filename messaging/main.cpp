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

#include <sstream>
#include <iostream>
#include <iomanip>
#include <dlib/serialize.h>
#include <dlib/iosockstream.h>
#include <dlib/matrix.h>
#include "include/message.hpp"

int main(int argc, char *argv[]){

  dlib::matrix<float,2,2> m;
  m = 1.0, 0.0, -1.0, 0.0;

  _internal::message msg("me", "recip");
  msg << m;
  zmqpp::message zmq_msg = msg.to_zmq();
  dlib::matrix<float> m_out;
  msg.from_zmq(zmq_msg);
  msg >> m_out;

  std::cout << m_out << std::endl;



  return 0;
}