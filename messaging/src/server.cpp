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


server::server(std::string keydir) : ctx_() {
  backend_ = new zmqpp::socket (ctx_, zmqpp::socket_type::router); //1 to N
  frontend_ = new zmqpp::socket (ctx_, zmqpp::socket_type::dealer); //N to 1

  //load up keys from file?
}
