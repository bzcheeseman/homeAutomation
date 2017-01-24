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

class server {

  //! ZMQ context
  zmqpp::context ctx_;

  //! ZMQ client facing port - encrypted with CURVE
  zmqpp::socket *frontend_;
  zmqpp::curve::keypair server_keypair_;
  std::vector<std::string> client_pubkeys_;

  //! ZMQ worker facing port
  zmqpp::socket *backend_;


public:
  server(std::string keydir);


};


#endif //HOMEAUTOMATION_SERVER_HPP
