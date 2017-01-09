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

//STL dependencies
#include <mutex>
#include <map>
#include <vector>
#include <cassert>

/*!
 * @struct logEntry_t
 *
 * Holds data to be entered into a log
 */
struct logEntry_t {
  //!Term in which entry was created
  size_t term;

  //!Entry unique id
  int id;

  //!Actual data in entry
  union{
    double d_data;
    float f_data;
    long l_data;
    int i_data;
    char* c_str_data;
  };

  enum {DOUBLE, FLOAT, LONG, INT, C_STR} data_type;

};

/*!
 * @struct entryResponse_t
 *
 * Holds the response from a long entry - the idx, id and term in which the entry was committed.
 */
struct entryResponse_t {
  //!Entry's unique ID
  int id;

  //!Term in which the entry was commited
  size_t term;

  //!Entry's index
  int idx;
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
class log {

private:
  /*!
   * Number of entries in the log
   * Must be equal to _log.size()
   */
  size_t log_count;

  /*!
   * The current term - this number controls a lot and will be incremented externally.
   *
   * add a mutex to this?
   */
  size_t current_term;

  /*!
   * Holds the entries in a map - allows us to change the keys without changing the order if required.
   *
   */
  std::map<size_t, logEntry_t> _log;

  /*!
   * Mutex to lock the log map
   *
   */
  std::mutex _log_mutex;

  /*!
   * Current (latest) entry idx
   */
  int current_idx;

public:
  //!Default constructor
  log();

  /*!
   * Copy assignment
   *
   * Locks the log mutexes of this object and the other object, so be cautious about locks when creating
   * or assigning logs to each other.
   *
   * @param other The log object to be copied
   * @returns log& Returns this object
   */
  log& operator=(log &other);

  /*!
   * Move assignment
   *
   * Locks the log mutexes of this object and the other object, so be cautious about locks when creating
   * or assigning logs to each other.
   *
   * @param other The log object to be moved
   * @returns log& Returns this object
   */
  log& operator=(log &&other);

  //!Copy constructor
  //!@see operator=(const log&)
  log(log &other);

  //!Move constructor
  //!@see operator=(log&&)
  log(log &&other);

  /*!
   * Default destructor
   * Locks _log_mutex, need to be sure to unlock this mutex before calling dtor!
   */
  virtual ~log();

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
  inline int getCurrent_idx() const { return current_idx; }

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
  entryResponse_t appendEntry(const logEntry_t &data);

  /*!
   * @copydoc appendEntry(const logEntry_t&)
   *
   * Allows us to take advantage of the return from a function call directly.
   */
  entryResponse_t appendEntry(logEntry_t &&data);

  ///////

  /*!
   * Gets the log entry at a certain idx safely.  Mostly identical to the map accessor
   * but with locks for thread safety.
   *
   * @param idx idx at which the data we want resides.
   *
   * @returns logEntry_t containing the data we wanted.
   *
   */
  logEntry_t & operator[](const int &idx);

  /*!
   * @copydoc operator[](const int&)
   *
   * Allows us to take advantage of the return value of a function.
   *
   */
  logEntry_t & operator[](int &&idx);

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
  std::vector<logEntry_t> getRange(const int start_idx, const int end_idx = -1);

};


#endif //HOMEAUTOMATION_LOG_HPP
