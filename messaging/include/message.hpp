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

//STL
#include <string>
#include <vector>
#include <cassert>

//DLIB
#include <dlib/serialize.h>

//ZMQ
#include <zmqpp/zmqpp.hpp>

//! @namespace _internal messaging/include/message.hpp
//! @brief holds internal methods that any API users will likely never interact with.
namespace _internal {

  /**
   * @class message messaging/include/message.hpp
   * @brief Holds an internal message handler. The user likely won't ever see this part at all.
   */
  class message {

    //! Recepient of this message - Human-readable format
    const std::string recipient;

    //! Sender of this message - Human-readable format
    const std::string sender;

    //! Contents of this message. Non-const but we still want it to be immutable.
    std::string contents;
  
  public:
    message(const std::string &recipient, 
            const std::string &sender) : recipient(recipient), sender(sender) {
      DLIB_CASSERT(recipient.size() != 0 && sender.size() != 0, "No recipient or sender defined!");
    }

    //! Delete copy constructor because I don't want to be able to copy a (potentially unique) message
    message(const message &other) = delete;

    //! Delete move constructor because we don't need it.
    message(message &&other) = delete;
  
    /**
     * Get recipient of message
     *
     * @return Recipient (std::string)
     */
    const std::string &get_recipient() const { return recipient; }

    /**
     * Get sender of a message
     *
     * @return Sender (std::string)
     */
    const std::string &get_sender() const { return sender; }
  
    /**
     * Sets the contents of a message. The current message payload must be empty.
     * @param payload
     */
    inline void set(std::string payload){
      DLIB_CASSERT(contents.empty(), "Overwriting message contents!");
      contents = payload;
    }

    /**
     * Ensures that you can get the contents of a message as a string, but keeps the actual object immutable.
     *
     * @return Copy of the contents of the message
     */
    inline std::string get(){
      DLIB_CASSERT(!contents.empty(), "Message is empty!");
      return contents;
    }

    /**
     * Sends the contents of a message to a ZMQ message.
     *
     * @return ZMQ message ready to send.
     */
    zmqpp::message to_zmq(){
      zmqpp::message msg;
      msg << this->contents;
      return msg;
    }

    /**
     * Reads in the contents of a ZMQ message
     *
     * @param msg A ZMQ message whose contents we are reading in
     */
    void from_zmq(zmqpp::message &msg){
      msg >> this->contents;
    }
  
    /**
     * Serializes an object to a string and places it into the contents of the specified message.
     * A thin wrapper around the dlib::serialize function.
     *
     * @tparam serializable A serializable type
     * @param out The message we are serializing into
     * @param obj The object we are serializing
     * @return The message we are serializing into
     */
    template<typename serializable>
    inline friend message &operator<<(message &out, serializable &obj){
      std::stringstream stream;
      dlib::serialize(obj, stream);
      out.set(stream.str());
      return out;
    }

    /**
     * Deserializes an object from the contents of the specified message.
     * A thin wrapper around the dlib::deserialize function.
     *
     * @tparam serializable A serializable type
     * @param in The message we are deserializing from
     * @param obj The object we are deserializing into
     * @return The object we are deserializing into
     */
    template<typename serializable>
    inline friend serializable &operator>>(message &in, serializable &obj){
      std::stringstream stream (in.get());
      dlib::deserialize(obj, stream);
      return obj;
    }
  
  };

}; //_internal


#endif //HOMEAUTOMATION_MESSAGE_HPP
