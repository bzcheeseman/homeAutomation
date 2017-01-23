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
#include "include/dlib_zmq.hpp"

int main(int argc, char *argv[]){

  dlib::matrix<float,2,2> m;
  m = 1.0, 0.0, -1.0, 0.0;

  //this process works! Just need to send it through zmq now :P

  zmqpp::message msg;
  dlib_zmq::to_zmq(m, msg);


//  std::stringstream ss;
//  std::ostream out(ss.rdbuf());
//
//  dlib::serialize(m, out); //serialize object to a stringstream
//
//  std::string serialized = ss.str(); //convert to a string
//  std::cout << serialized << std::endl;
//
//  std::cout << serialized.size() << std::endl;
//  std::cout << (int)serialized[0] << std::endl;
//  std::cout << (int)serialized[1] << std::endl;
//
//  auto inbackupbuf = std::cin.rdbuf(ss.rdbuf()); //redirect to cin now
//  std::stringstream back_in(serialized); //back to a stringstream
  dlib::matrix<float> m_out;
  dlib_zmq::from_zmq(m_out, msg);
//
//  dlib::deserialize(m_out, back_in); //and deserialize
//
//  std::cout.rdbuf(backupbuf);

  std::cout << m_out << std::endl;



  return 0;
}