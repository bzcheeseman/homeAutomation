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


#include "../include/data_log.hpp"

/*!
 * @file log.cpp
 * @brief Holds the function definitions for log.hpp
 *
 * Defines all the functions not defined in log.hpp.  Also has quite a few valuable notes on implementation
 * that should be included in the documentation.
 */

//Default ctor
data_log::data_log(): log_count(0),
            current_idx(0) {
  ;
}

//Copy assgmt
data_log &data_log::operator=(data_log &other) {
  //Lock mutexes
  std::lock(_log_mutex, other._log_mutex);

  //Adopt the locks for RAII style locking
  std::lock_guard<std::mutex> my_lock(_log_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> other_lock(other._log_mutex, std::adopt_lock);

  //Perform operations
  log_count = other.log_count;
  current_idx = other.current_idx;
  _str_log = other._str_log;
  _img_log = other._img_log;
  _grey_img_log = other._grey_img_log;

  return *this;
}

//Move assgmt
data_log &data_log::operator=(data_log &&other) {
  //Lock mutexes
  std::lock(_log_mutex, other._log_mutex);

  //Adopt locks for RAII style locking
  std::lock_guard<std::mutex> my_lock(_log_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> other_lock(other._log_mutex, std::adopt_lock);

  //Perform operations
  log_count = other.log_count;
  current_idx = other.current_idx;
  _str_log = other._str_log;
  _img_log = other._img_log;
  _grey_img_log = other._grey_img_log;

  //Release other.lock to delete it (the dtor calls a lock itself)
  other_lock.~lock_guard();

  return *this;
}

//Copy ctor
data_log::data_log(data_log &other){
  //Reuse assignment operator
  *this = other;
}

//Move ctor
data_log::data_log(data_log &&other){
  //Reuse assignment operator
  *this = other;
}

//Default dtor
data_log::~data_log() {
  this->clearLog();
}

//Clears the data_log
void data_log::clearLog() {
  std::lock_guard<std::mutex> lock(_log_mutex);
  log_count = 0;
  current_idx = 0;
  _str_log.clear();
  _img_log.clear();
  _grey_img_log.clear();
}


entryResponse_t data_log::appendEntry(const log_entry &data) {

  entryResponse_t out;
  out.term = current_term;
  out.idx = current_idx;

  std::lock_guard<std::mutex> lock(_log_mutex); //thread safety!

  switch (data.data_type){
    case log_entry::STR : _str_log[current_idx] = data.str_data; break;
    case log_entry::IMG : _img_log[current_idx] = data.img_data; break;
    case log_entry::G_IMG : _grey_img_log[current_idx] = data.grey_img_data; break;
  }


  log_count++;
  current_idx++;

  return out;
}

entryResponse_t data_log::appendEntry(log_entry &&data) {

  entryResponse_t out;
  out.term = current_term;
  out.idx = current_idx;

  std::lock_guard<std::mutex> lock(_log_mutex); //thread safety!

  switch (data.data_type){
    case log_entry::STR : _str_log[current_idx] = data.str_data; break;
    case log_entry::IMG : _img_log[current_idx] = data.img_data; break;
    case log_entry::G_IMG : _grey_img_log[current_idx] = data.grey_img_data; break;
  }

  log_count++;
  current_idx++;

  return out;
}

log_entry &data_log::operator()(const size_t &idx, data_log::data_type which) {
  assert(idx <= current_idx);

  std::lock_guard<std::mutex> lock(_log_mutex);

  log_entry out;

  switch (which){
    case STR : out.str_data = _str_log[idx]; out.data_type = log_entry::STR; break;
    case IMG : out.img_data = _img_log[idx]; out.data_type = log_entry::IMG; break;
    case G_IMG : out.grey_img_data = _grey_img_log[idx]; out.data_type = log_entry::G_IMG; break;
  }

  return out;

}

log_entry &data_log::operator()(size_t &&idx, data_log::data_type which) {
  assert(idx <= current_idx);

  std::lock_guard<std::mutex> lock(_log_mutex);

  log_entry out;

  switch (which){
    case STR : out.str_data = _str_log[idx]; out.data_type = log_entry::STR; break;
    case IMG : out.img_data = _img_log[idx]; out.data_type = log_entry::IMG; break;
    case G_IMG : out.grey_img_data = _grey_img_log[idx]; out.data_type = log_entry::G_IMG; break;
  }

  return out;
}

std::vector<log_entry> data_log::getRange(data_log::data_type which, const size_t start_idx, const size_t end_idx) {
  std::vector<log_entry> out;

  assert(start_idx < current_idx && (start_idx < end_idx || end_idx == 0));

  std::lock_guard<std::mutex> lock(_log_mutex);

  size_t end;

  if (end_idx == 0){
    end = current_idx;
  }
  else{
    end = end_idx;
  }

  log_entry temp;
  for (size_t i = start_idx; i <= end; i++){
    temp = (*this)(i, which);
    out.push_back(temp);
  }

  return out;

}

std::vector<log_entry> data_log::allofType(data_log::data_type which) {
  std::vector<log_entry> out;

  log_entry temp;
  for (size_t i = 0; i <= current_idx; i++){
    temp = (*this)(i, which);
    out.push_back(temp);
  }

  return out;
}

void data_log::dumpLogs(std::string filename) {
  filename += std::to_string(current_idx);

  std::lock_guard<std::mutex> lock(_log_mutex);

  dlib::serialize(filename + "_img.log") << _img_log;
  dlib::serialize(filename + "_grey_img.log") << _grey_img_log;
  dlib::serialize(filename + "_str.log") << _str_log;


  return;
}






