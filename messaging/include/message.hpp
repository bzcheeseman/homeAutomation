//
// Created by Aman LaChapelle on 1/24/17.
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


#ifndef HOMEAUTOMATION_MESSAGE_HPP
#define HOMEAUTOMATION_MESSAGE_HPP

#include <string>
#include <vector>
#include <cassert>

#include <dlib/serialize.h>

#include <zmqpp/zmqpp.hpp>

#include "dlib_zmq.hpp"

namespace messaging {

  class message {
  
    const std::string recipient;
    const std::string sender;
    std::vector<std::string> contents;
  
  public:
    message(const std::string &recipient, 
            const std::string &sender) : recipient(recipient), sender(sender) {}
  
    //Use this to set source and destination for zmq.
    //Guarantees that the recipient and sender for a given message cannot change.
    const std::string &get_recipient() const { return recipient; }
    const std::string &get_sender() const { return sender; }
  
    //Modifies the contents of the message
    inline void add(std::string payload){ contents.push_back(payload); }
    inline std::string &get(int which){ return contents[which]; }
    inline void remove(int which){ 
      auto iter_which = contents.begin() + which; 
      contents.erase(iter_which); 
    }
  
    //If there's only one item in the message, we can access it directly as a string if we so choose.
    inline std::string &str(){ assert(contents.size() == 1); return contents[0]; }
  
    //Note these are do NOT operate on a heap data structure, just thought they might come in handy.
    inline void push(std::string payload){ this->add(payload); }
    inline std::string &pop(){ return *(contents.end()); }
  
    //Serialize a message to send over a zmq socket. The ostream can be a stringstream or something, that's kosher.
    //Note that this ONLY sends the contents, recipient/sender needs to be taken care of outside of this.
    friend zmqpp::message &operator>>(zmqpp::message &out, message &msg){ //check on these insertion operators
      dlib_zmq::to_zmq(msg.contents, out);
      return out;
    }
    
    friend message operator<<(message &out, zmqpp::message &msg){ //check on these insertion operators
      dlib_zmq::from_zmq(out.contents(), msg);
      return out;
    }
  
  };

}; //messaging


#endif //HOMEAUTOMATION_MESSAGE_HPP
