//
// Created by Aman LaChapelle on 1/8/17.
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


#ifndef HOMEAUTOMATION_COMMUNICATE_HPP
#define HOMEAUTOMATION_COMMUNICATE_HPP

//dlib dependencies
#include <dlib/dnn.h>

//STL dependencies
#include <tuple>

//Project dependencies
#include "../../utilities/py_module.hpp"
#include "log.hpp"

template<class... Types>
class message_t {

private:
  const std::tuple<Types...> message;
  const size_t term;

public:
  message_t(const std::tuple<Types...> &message, const size_t &term) : message(message), term(term) {}
  message_t(std::tuple<Types...> &&message, size_t &&term) : message(message), term(term) {}

  inline const size_t getTerm() const { return term; }

};

class device {

private:
  std::string name; //What we're calling the device, should be human-readable like 'Arduino board 1' or something
  std::string unique_id; //IP address, MAC address, etc.
  size_t disk_available; //Amount of memory still available on the disk in kB
  size_t ram_available; //RAM available on the device in kB
  bool gpu_enabled; //Whether or not the device has a CUDA gpu on it.

public:
  device(const std::string &name, const std::string &unique_id, const bool gpu_enabled);

  virtual ~device();

  size_t getDisk_available() const;

  size_t getRam_available() const;

  bool isGpu_enabled() const;


  //TODO: Initialize connection (probably with Python)
  //TODO: Send Information
  //TODO: Close connection
};


#endif //HOMEAUTOMATION_COMMUNICATE_HPP