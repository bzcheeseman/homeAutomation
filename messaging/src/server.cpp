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


#include "../include/server.hpp"

worker::worker(zmqpp::context &ctx) : ctx_(ctx), worker_(ctx_, ZMQ_PULL) { }

void worker::work() {

  worker_.connect("inproc://backend");

  try {
    zmqpp::message identity;
    zmqpp::message msg;
    zmqpp::message copied_identity;
    zmqpp::message reply_msg;

    worker_.receive(identity);
    worker_.receive(msg);

    //process the message and do stuff. Then reply to the server.

  }
  catch(std::exception &e){
    std::cout << e.what() << std::endl;
  }


}

