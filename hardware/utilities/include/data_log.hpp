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


#ifndef HOMEAUTOMATION_LOG_HPP
#define HOMEAUTOMATION_LOG_HPP

/*
 * TODO: Modify this to work with the device code that I end up using...
 * TODO: Use ZMQ to communicate between devices!
 */

//dlib dependencies
#include <dlib/serialize.h>
#include <dlib/dnn.h>

//STL dependencies
#include <mutex>
#include <map>
#include <vector>
#include <cassert>

//0MQ dependencies
#include <zmq.h>

struct log_entry {
  std::string str_data;
  dlib::matrix<dlib::rgb_pixel> img_data;
  dlib::matrix<unsigned char> grey_img_data;

  enum {STR, IMG, G_IMG} data_type ;

  log_entry(){};
  log_entry(std::string data) : str_data(data), data_type(STR) {}
  log_entry(dlib::matrix<dlib::rgb_pixel> data) : img_data(data), data_type(IMG) {}
  log_entry(dlib::matrix<unsigned char> data) : grey_img_data(data), data_type(G_IMG) {}
};



/*!
 * @file log.hpp
 * @brief Holds the log and methods pertaining to it.
 * @version 0.1
 *
 * Each instance will hold the log for any given node.  This contains the relevant functions for appending to the log,
 * clearing the log, and reading from the log.
 *
 * This class is hopefully thread-safe and RAII compliant.  It's supposed to be.
 *
 * @class log include/log.hpp src/log.hpp
 * @brief Holds the log and methods pertaining to it.
 *
 * Each instance will hold the log for any given node.  This contains the relevant functions for appending to the log,
 * clearing the log, and reading from the log.
 *
 * This class is hopefully thread-safe.  It's supposed to be.
 */
class data_log {

private:
  size_t log_count;

  size_t current_term;

  std::map< unsigned long, std::string> _str_log; //change these to have keys that are their labels? That way I can train on this stuff

  std::map< unsigned long, dlib::matrix<dlib::rgb_pixel> > _img_log;

  std::map< unsigned long, dlib::matrix<unsigned char> > _grey_img_log;

  std::mutex _log_mutex;

  unsigned long current_label;

public:
  //!Default constructor
  data_log();

  /*!
   * Copy assignment
   *
   * Locks the log mutexes of this object and the other object, so be cautious about locks when creating
   * or assigning logs to each other.
   *
   * @param other The log object to be copied
   * @returns log& Returns this object
   */
  data_log &operator=(data_log &other);

  /*!
   * Move assignment
   *
   * Locks the log mutexes of this object and the other object, so be cautious about locks when creating
   * or assigning logs to each other.
   *
   * @param other The log object to be moved
   * @returns log& Returns this object
   */
  data_log &operator=(data_log &&other);

  //!Copy constructor
  //!@see operator=(const log&)
  data_log(data_log &other);

  //!Move constructor
  //!@see operator=(log&&)
  data_log(data_log &&other);

  /*!
   * Default destructor
   * Locks _log_mutex, need to be sure to unlock this mutex before calling dtor!
   */
  virtual ~data_log();

  /*!
   * To clear the log without destroying the object.
   * Locks _log_mutex, need to be sure it's unlocked before calling function
   *
   * @param None
   *
   * @returns None
   */
  void clearLog();

  ////////

  /*!
   * Increments the current term.  Nothing else.
   */
  inline void incrementTerm() { current_term++; }

  /*!
   * Getter for current_idx
   *
   * @param None
   *
   * @returns current_idx
   */
  inline size_t getCurrent_idx() const { return current_idx; }

  /*!
   * Getter for log_count
   *
   * @param None
   *
   * @returns log_count
   */
  inline size_t getlog_count() const { return log_count; }

  ///////

  /*!
   * Adds an entry to the log in a (hopefully) thread safe way.
   *
   * @param data The data to be added to the log
   *
   * @returns The proper response, containing the entry's ID and idx, and term in which it was committed
   */
  entryResponse_t appendEntry(const log_entry &data);

  /*!
   * @copydoc appendEntry(const logEntry_t&)
   *
   * Allows us to take advantage of the return from a function call directly.
   */
  entryResponse_t appendEntry(log_entry &&data);

  ///////

  enum data_type {STR, IMG, G_IMG};

  /*!
   * Gets the log entry at a certain idx safely.  Mostly identical to the map accessor
   * but with locks for thread safety.
   *
   * @param idx idx at which the data we want resides.
   *
   * @returns logEntry_t containing the data we wanted.
   *
   */
  log_entry &operator()(const size_t &idx, data_type which);

  /*!
   * @copydoc operator[](const int&)
   *
   * Allows us to take advantage of the return value of a function.
   *
   */
  log_entry &operator()(size_t &&idx, data_type which);

  ///////

  /*!
   * Gets the log entry at a certain idx.
   *
   * IMPORTANT!!!! This is awkward in that we need all the data in the range we want to have the same type -
   * so the user needs to check this for now. - might not be an issue with the redesign.
   *
   * @param num_entries Holds the number of entries contained
   * @param start_idx Where we start the range
   * @param end_idx Where we end the range - defaults to -1 which is the end.
   *
   * @returns Array of log entries
   *
   */
  std::vector<log_entry> getRange(data_type which, const size_t start_idx, const size_t end_idx = 0);

  std::vector<log_entry> allofType(data_type which); //change this to allString, allImg, allGImg and just return a vector of the type.

  void dumpLogs(std::string filename = "../../logging/logs_");

  inline size_t operator>(const data_log &other){
    return (this->current_term > other.current_term) ? this->current_term : other.current_term;
  }

  inline size_t operator<(const data_log &other){
    return (this->current_term < other.current_term) ? this->current_term : other.current_term;
  }

};


#endif //HOMEAUTOMATION_LOG_HPP
