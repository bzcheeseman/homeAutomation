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


#include "../include/communicate.hpp"

device::device(const std::string &name, const std::string &unique_id, const bool gpu_enabled) :
        name(name), unique_id(unique_id), gpu_enabled(gpu_enabled) {}

device::~device() {
  ;
}


size_t device::getDisk_available() const {
  return disk_available;
}

size_t device::getRam_available() const {
  return ram_available;
}

bool device::isGpu_enabled() const {
  return gpu_enabled;
}


