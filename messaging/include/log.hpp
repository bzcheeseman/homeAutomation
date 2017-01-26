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

#include <dlib/matrix.h>
#include <dlib/assert.h>
#include <dlib/pixel.h>
#include <dlib/algs.h>

namespace _internal {

  struct _entry {
    //Data stuff
    std::string str_data;
    dlib::matrix<dlib::rgb_pixel> img_data;
    dlib::matrix<unsigned char> grey_img_data;
    enum {STR, IMG, G_IMG} data_type;

    _entry(){};
    _entry(std::string data) : str_data(data), data_type(STR) {}
    _entry(dlib::matrix<dlib::rgb_pixel> data) : img_data(data), data_type(IMG) {}
    _entry(dlib::matrix<unsigned char> data) : grey_img_data(data), data_type(G_IMG) {}

    //Metadata (is the label human-verified? Is there a label?)
    std::string label;
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

  };

  //basically a raft log for the server so that I can store old pictures and add them to the training
  class log {
    std::vector<_entry> entries;

    size_t current_term; //The most recent term this log has. Every time a new entry is added,

  public: //deal with labeled vs unlabeled? Should everything be labeled?
    void add_entry(_entry &entry){ //inserts an already constructed entry type
      entries.push_back(entry);
    }

    template<typename type>
    void add_entry(type &obj){
      bool correct_type = dlib::is_same_type<type, std::string>::value ||
                          dlib::is_same_type<type, dlib::matrix<dlib::rgb_pixel>>::value ||
                          dlib::is_same_type<type, dlib::matrix<unsigned char>>::value;

      DLIB_CASSERT(correct_type, "Data type does not exist in the log!");

      entries.push_back(_entry(obj)); //inserts a new entry from raw data
    }

    std::vector<_entry> get_unverified(){
      std::vector<_entry> out;
      for (auto iter = entries.begin(); iter != entries.end(); iter++){
        if (!(iter->verified)){
          out.push_back(*iter);
        }
      }
      return out;
    }

    std::vector<_entry> get_verified(){
      std::vector<_entry> out;
      for (auto iter = entries.begin(); iter != entries.end(); iter++){
        if (iter->verified){
          out.push_back(*iter);
        }
      }
      return out;
    }

    template<typename type>
    void get_all_type(std::vector<type> &vec){

      bool correct_type = dlib::is_same_type<type, std::string>::value ||
                          dlib::is_same_type<type, dlib::matrix<dlib::rgb_pixel>>::value ||
                          dlib::is_same_type<type, dlib::matrix<unsigned char>>::value;

      DLIB_CASSERT(correct_type, "Data type does not exist in the log!");

      if (dlib::is_same_type<type, std::string>::value){
        for (auto iter = entries.begin(); iter != entries.end(); iter++){
          if (iter->data_type == _entry::STR){
            vec.push_back(*iter);
          }
        }
      }
      else if (dlib::is_same_type<type, dlib::matrix<dlib::rgb_pixel>>::value){
        for (auto iter = entries.begin(); iter != entries.end(); iter++){
          if (iter->data_type == _entry::IMG){
            vec.push_back(*iter);
          }
        }
      }
      else if (dlib::is_same_type<type, dlib::matrix<unsigned char>>::value){
        for (auto iter = entries.begin(); iter != entries.end(); iter++){
          if (iter->data_type == _entry::G_IMG){
            vec.push_back(*iter);
          }
        }
      }
    }

    //comparison operators?

    friend void serialize(log &l, std::ostream &out){
      for (auto iter = l.entries.begin(); iter != l.entries.end(); iter++){
        serialize(*iter, out);
      }
    }



  };

}; //_internal




#endif //HOMEAUTOMATION_LOG_HPP
