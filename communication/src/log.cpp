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


#include "../include/log.hpp"

/*!
 * @file log.cpp
 * @brief Holds the function definitions for log.hpp
 *
 * Defines all the functions not defined in log.hpp.  Also has quite a few valuable notes on implementation
 * that should be included in the documentation.
 */

//Default ctor
log::log(): log_count(0),
            current_idx(0) {
  ;
}

//Copy assgmt
log &log::operator=(log &other) {
  //Lock mutexes
  std::lock(_log_mutex, other._log_mutex);

  //Adopt the locks for RAII style locking
  std::lock_guard<std::mutex> my_lock(_log_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> other_lock(other._log_mutex, std::adopt_lock);

  //Perform operations
  log_count = other.log_count;
  current_idx = other.current_idx;
  _log = other._log;

  return *this;
}

//Move assgmt
log &log::operator=(log &&other) {
  //Lock mutexes
  std::lock(_log_mutex, other._log_mutex);

  //Adopt locks for RAII style locking
  std::lock_guard<std::mutex> my_lock(_log_mutex, std::adopt_lock);
  std::lock_guard<std::mutex> other_lock(other._log_mutex, std::adopt_lock);

  //Perform operations
  log_count = other.log_count;
  current_idx = other.current_idx;
  _log = other._log;

  //Release other.lock to delete it (the dtor calls a lock itself)
  other_lock.~lock_guard();
  //Delete other
  other.~log();

  return *this;
}

//Copy ctor
log::log(log &other){
  //Reuse assignment operator
  *this = other;
}

//Move ctor
log::log(log &&other){
  //Reuse assignment operator
  *this = other;
}

//Default dtor
log::~log() {
  this->clearLog();
}

//Clears the log
void log::clearLog() {
  std::lock_guard<std::mutex> lock(_log_mutex);
  log_count = 0;
  current_idx = 0;
  _log.clear();
}


entryResponse_t log::appendEntry(const logEntry_t &data) {

  entryResponse_t out;
  out.id = data.id;
  out.term = current_term;
  out.idx = current_idx;

  std::lock_guard<std::mutex> lock(_log_mutex); //thread safety!

  _log[current_idx] = data;
  log_count++;
  current_idx++;

  return out;
}

entryResponse_t log::appendEntry(logEntry_t &&data) {

  entryResponse_t out;
  out.id = data.id;
  out.term = current_term;
  out.idx = current_idx;

  std::lock_guard<std::mutex> lock(_log_mutex); //thread safety!
  _log[current_idx] = data;
  log_count++;
  current_idx++;

  return out;
}

logEntry_t &log::operator[](const int &idx) {
  assert(idx <= current_idx);

  std::lock_guard<std::mutex> lock(_log_mutex);
  return _log[idx];
}

logEntry_t &log::operator[](int &&idx) {
  assert(idx <= current_idx);

  std::lock_guard<std::mutex> lock(_log_mutex);
  return _log[idx];
}

std::vector<logEntry_t> log::getRange(const int start_idx, const int end_idx = -1) {

  std::vector<logEntry_t> out;

  assert(start_idx < current_idx);

  std::lock_guard<std::mutex> lock(_log_mutex);

  if (end_idx == -1){
    for (int i = start_idx; i <= current_idx; i++){
      out.push_back(_log[i]);
    }
  }
  else{
    for (int i = start_idx; i <= end_idx; i++){
      out.push_back(_log[i]);
    }
  }

  return out;
}