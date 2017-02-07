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

/*
 * TODO: Add a to_log function? Do I want to send these messages to a log directly?
 */

//! @namespace _internal messaging/include/message.hpp
//! @brief holds internal methods that any API users will likely never interact with.
namespace _internal {

  /**
   * @class message messaging/include/message.hpp
   * @brief Holds an internal message handler. The user likely won't ever see this part at all.
   */
  class message {

  protected:
    //! Recepient of this message - Human-readable format
    const std::string recipient;

    //! Sender of this message - Human-readable format
    const std::string sender;

  private:
    //! The type information of the contents of the message.
    std::string contents_type;

    //! Contents of this message. Non-const but we still want it to be immutable.
    std::string contents;
  
  public:
    message(const std::string &recipient, 
            const std::string &sender) : recipient(recipient), sender(sender) {
      DLIB_CASSERT(recipient.size() != 0 && sender.size() != 0, "No recipient or sender defined!");
    }

    //! Delete copy constructor because I don't want to be able to copy a (potentially unique) message
    message(const message &other) = delete;

    //! Set up move constructor for logging
    message(message &&other) : recipient(other.recipient), sender(other.sender), contents(other.contents) {}

    //add to_log(log &l) that adds the message to the log

    //add from_log that sets the contents of the message from a log entry

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
    inline virtual void set_contents(std::string payload){
      DLIB_CASSERT(contents.empty(), "Overwriting message contents not allowed!");
      contents = payload;
    }

    /**
     * Ensures that you can get the contents of a message as a string, but keeps the actual object immutable.
     *
     * @return Copy of the contents of the message
     */
    inline virtual std::string get_contents(){
      DLIB_CASSERT(!contents.empty(), "Message is empty!");
      return contents;
    }

    /**
     * Sends the contents of a message to a ZMQ message.
     *
     * @return ZMQ message ready to send.
     */
    inline virtual zmqpp::message to_zmq(){
      zmqpp::message msg;
      msg << this->contents_type << this->contents;
      return msg;
    }

    /**
     * Sends the contents of a message to a ZMQ message.
     *
     * @param msg The message (already constructed) to put values into.
     */
    inline virtual void to_zmq(zmqpp::message &msg){
      DLIB_CASSERT(msg.size(0) == 0, "Overwriting message contents!");
      msg << this->contents_type << this->contents;
    }

    /**
     * Reads in the contents of a ZMQ message
     *
     * @param msg A ZMQ message whose contents we are reading in
     */
    inline virtual void from_zmq(zmqpp::message &msg){
      msg >> this->contents_type >> this->contents;
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
      out.set_contents(stream.str());
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
      std::stringstream stream (in.get_contents());
      dlib::deserialize(obj, stream);
      return obj;
    }
  
  };

  /**
   * @class key_swap_msg messaging/include/message.hpp
   * @brief Holds an internal public key message handler. The user likely won't ever see this part at all.
   *
   * This type of message will be the only kind of message that we will send with no encryption, everything else is
   * encrypted using one of these. Also makes it easier to ensure that the key swap took place already.
   */
  class key_swap_msg : public message {

    //! The public key that we want to send.
    std::string my_public_key;

  public:
    key_swap_msg(const std::string &recipient,
                 const std::string &sender) : message(recipient, sender) {}

    /**
     * Sets the contents of the message (a public key) to the public member of a keypair.
     *
     * @param pubkey The public key to set in the message.
     */
    inline void set_contents(std::string pubkey){
      this->my_public_key = pubkey;
    }

    /**
     * Gets the public key contained in this message.
     *
     * @return A string that is equal to the public key contained in the message.
     */
    inline std::string get_contents() {
      DLIB_CASSERT(!my_public_key.empty(), "No key contained!");
      return my_public_key;
    }

    /**
     * Sends the public key to a ZMQ message.
     *
     * @return ZMQ message ready to send.
     */
    inline zmqpp::message to_zmq(){
      zmqpp::message msg;
      msg << "pubkey" << this->my_public_key;
      return msg;
    }

    /**
     * Sends the public key to a ZMQ message.
     *
     * @param msg The message (already constructed) to put values into.
     */
    inline void to_zmq(zmqpp::message &msg){
      DLIB_CASSERT(msg.size(0) == 0, "Overwriting message contents!");
      msg << "pubkey" << this->my_public_key;
    }

    /**
     * Reads in the contents of a ZMQ message
     *
     * @param msg A ZMQ message whose contents we are reading in
     */
    inline virtual void from_zmq(zmqpp::message &msg){
      std::string key;
      msg >> key >> this->my_public_key;
      DLIB_ASSERT(key == "pubkey", "This message did not contain a public key!");
    }

  };

}; //_internal


#endif //HOMEAUTOMATION_MESSAGE_HPP
