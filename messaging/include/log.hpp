//
// Created by Aman LaChapelle on 1/25/17.
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

#include <string>
#include <map>
#include <vector>
#include <thread>

#include <dlib/matrix.h>
#include <dlib/assert.h>
#include <dlib/pixel.h>
#include <dlib/algs.h>

#include "message.hpp"

namespace _internal {

  struct _entry {

    _entry(){};
    _entry(std::string data) : str_data(data), data_type(STR), verified(false) {}
    _entry(dlib::matrix<dlib::rgb_pixel> data) : img_data(data), data_type(IMG), verified(false) {}
    _entry(dlib::matrix<unsigned char> data) : grey_img_data(data), data_type(G_IMG), verified(false) {}

    //Data
    std::string str_data;
    dlib::matrix<dlib::rgb_pixel> img_data;
    dlib::matrix<unsigned char> grey_img_data;
    enum {STR, IMG, G_IMG} data_type;

    //Metadata
    std::string label; //There needs to be a label of some sort, if it's just information then the label can be "__LOG__" or something
    bool verified;

    size_t term_added; //server heartbeat term. Everyone increments this every time the server sends out a heartbeat.

    //Serialization support
    friend void serialize(_entry &e, std::ostream &out){
      switch(e.data_type){
        case STR : dlib::serialize(e.str_data, out);
        case IMG : dlib::serialize(e.img_data, out);
        case G_IMG : dlib::serialize(e.grey_img_data, out);
      }
    }

    friend void deserialize(_entry &e, std::istream &in){
      switch(e.data_type){
        case STR : dlib::deserialize(e.str_data, in);
        case IMG : dlib::deserialize(e.img_data, in);
        case G_IMG : dlib::deserialize(e.grey_img_data, in);
      }
    }

  };

  struct entry : public _entry {
    const std::string str_data;
    const dlib::matrix<dlib::rgb_pixel> img_data;
    const dlib::matrix<unsigned char> grey_img_data;
    const enum {STR, IMG, G_IMG} data_type;

    //Metadata
    const std::string label;
    const bool verified;

    const size_t term_added;

    entry(_entry &e) : str_data(e.str_data), img_data(e.img_data),
                       grey_img_data(e.grey_img_data), data_type(e.data_type),
                       label(e.label), verified(e.verified), term_added(e.term_added) {}
  };

  //basically a raft log for the server so that I can store old pictures and add them to the training
  class log {
    std::mutex log_mutex;
    std::vector<_entry> entries;

    size_t current_term; //The most recent term this log has.

  public: //add copy constructor
    // Adding entries to the log
    inline void add_entry(_entry &entry){ //inserts an already constructed entry type
      DLIB_CASSERT(!entry.label.empty(), "This sample is unlabeled!");
      std::lock_guard lock(log_mutex);
      entries.push_back(entry);
    }

    template<typename type>
    inline void add_entry(type &obj, std::string label){ //make sure everything is labeled
    
      bool correct_type = dlib::is_same_type<type, std::string>::value ||
                          dlib::is_same_type<type, dlib::matrix<dlib::rgb_pixel>>::value ||
                          dlib::is_same_type<type, dlib::matrix<unsigned char>>::value;

      DLIB_CASSERT(correct_type, "Data type does not exist in the log!");
      DLIB_CASSERT(!label.empty(), "This sample is unlabeled!");

      _entry e(obj);
      e.label = label;
      e.term_added = this->current_term;

      std::lock_guard lock(log_mutex);
      entries.push_back(e); //inserts a new entry from raw data
      
    }
    
    inline void push_back(_entry &entry){
      this->add_entry(entry);
    }
    
    template<typename type>
    inline void push_back(type &obj, std::string label){
      this->add_entry(obj, label);
    }

    // Retreiving data from the log
    inline std::vector<entry> get_unverified(){
      std::vector<entry> out(0);
      for (auto iter = entries.begin(); iter != entries.end(); iter++){
        if (!(iter->verified)){
          out.push_back(entry(*iter));
        }
      }
      return out;
    }

    inline std::vector<entry> get_verified(){
      std::vector<entry> out(0);
      for (auto iter = entries.begin(); iter != entries.end(); iter++){
        if (iter->verified){
          out.push_back(entry(*iter));
        }
      }
      return out;
    }

    //Access/subscript operator
    inline entry operator[](size_t which){
      return entry(entries[which]);
    }

    template<typename type>
    inline void get_all_of_type(std::vector<type> &vec){

      bool correct_type = dlib::is_same_type<type, std::string>::value ||
                          dlib::is_same_type<type, dlib::matrix<dlib::rgb_pixel>>::value ||
                          dlib::is_same_type<type, dlib::matrix<unsigned char>>::value;

      DLIB_CASSERT(correct_type, "Data type does not exist in the log!");

      if (dlib::is_same_type<type, std::string>::value){
        for (auto iter = entries.begin(); iter != entries.end(); iter++){
          if (iter->data_type == _entry::STR){
            vec.push_back(iter->str_data);
          }
        }
      }
      else if (dlib::is_same_type<type, dlib::matrix<dlib::rgb_pixel>>::value){
        for (auto iter = entries.begin(); iter != entries.end(); iter++){
          if (iter->data_type == _entry::IMG){
            vec.push_back(iter->img_data);
          }
        }
      }
      else if (dlib::is_same_type<type, dlib::matrix<unsigned char>>::value){
        for (auto iter = entries.begin(); iter != entries.end(); iter++){
          if (iter->data_type == _entry::G_IMG){
            vec.push_back(iter->grey_img_data);
          }
        }
      }
    }

    // Relational operators - based on current term
    inline friend bool operator<(const log &lhs, const log &rhs){ return lhs.current_term < rhs.current_term; }

    inline friend bool operator>(const log &lhs, const log &rhs){ return rhs < lhs; }

    inline bool operator==(const log &lhs, const log &rhs){ return lhs.current_term == rhs.current_term; }

    inline bool operator!=(const log &lhs, const log &rhs){ return lhs.current_term != rhs.current_term; }

    //Serialization support
    inline friend void serialize(log &l, std::ostream &out){
      for (auto iter = l.entries.begin(); iter != l.entries.end(); iter++){
        serialize(*iter, out);
      }
    }

    inline friend void serialize(std::vector<entry> &v, std::ostream &out){
      for (auto iter = v.begin(); iter != v.end(); iter++){
        serialize(*iter, out);
      }
    }

  };

}; //_internal




#endif //HOMEAUTOMATION_LOG_HPP
