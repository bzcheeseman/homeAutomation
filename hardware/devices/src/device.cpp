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


#include "../include/device.hpp"

device::device(const std::string &name,
               const std::string &unique_id,
               const bool gpu_enabled,
               const size_t num_threads) :
        name(name), unique_id(unique_id), gpu_enabled(gpu_enabled), num_threads(num_threads),
        pool(num_threads), my_log() {

  pycpp::which_python = "/usr/local/bin/python";

  slack = new pycpp::py_module ("slack", "../../python/scripts");
}

device::~device() {
  ;
}

size_t device::getDisk_available() const {
  return disk_available;
}

size_t device::getRam_available() const {
  return ram_available;
}

const std::string &device::getName() const {
  return name;
}

const bool device::isGpu_enabled() const {
  return gpu_enabled;
}

const std::string &device::getUnique_id() const {
  return unique_id;
}

const size_t device::getNum_threads() const {
  return num_threads;
}

void device::run_slack_server() {

  char *token = getenv("SLACK_KEY");

  pycpp::py_module slack_server = slack->py_class("SlackServer", pycpp::make_tuple(1, pycpp::to_python(token)));

  std::ifstream cmds_in;
  std::string line;

  while (PyFloat_AsDouble(slack_server("handle", 0)) != -1.0){
    cmds_in.open("/Users/Aman/code/homeAutomation/logging/commands.log"); //open the file
    while (getline(cmds_in, line)){ //read in all the lines
      cmds_in >> line;
      my_log.appendEntry(log_entry(line));
    }
    cmds_in.close(); //close the file

    my_log.incrementTerm(); //increment the term in the log and sleep for 100 ms.
    dlib::sleep(100);
  }

}

void device::share_logs(device &other) {
  if (other.my_log > this->my_log){
    this->my_log = other.my_log;
    return;
  }
  else if (this->my_log > other.my_log){
    other.my_log = this->my_log;
    return;
  }
  else{
    return;
  }
}
