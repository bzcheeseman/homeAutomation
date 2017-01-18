//
// Created by Aman LaChapelle on 8/14/16.
//

/*
    homeAutomation
    Copyright (C) 2016  Aman LaChapelle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 */

#ifndef HOMEAUTOMATION_SLACKINTERFACE_H
#define HOMEAUTOMATION_SLACKINTERFACE_H

#include <Python.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @file slackInterface.h
 * @brief Holds the slack interface - written in python - converted to C via python interpreter embedding.
 *
 * Binds the python script to C functions so they are callable from C.  C is chosen for being lightweight and fast.
 * This is meant to run on a RPi type computer, so it needs to be fast and lightweight.
 */


/**
 * @struct interface_t
 *
 * Holds:
 *  - The interface object that we use to make function calls
 *  - Whether the interface is initialized
 *  - The current term of the interface - increments by one each time a message is received/processed
 */
typedef struct {
  //!Pointer to the python interface object
  PyObject *interface;
  //!Whether or not the interface is initialized
  int initialized;
  //!The current term - basically number of messages since start but also just keeps track of time passage
  int current_term;
} interface_t ;

/**
 * Initializes the interpreter.  Must be careful to call the shutdownInterface function.
 *
 * @param interface The interface struct that holds the PyObject*, whether it's initialized, and the current term.
 */
void importInterface(interface_t *interface);

/**
 * Shuts down the interface, calling Py_Finalize().  (Should modify to free interface_t pointer too?
 *
 * @param interface The interface struct that holds the PyObject*, whether it's initialized, and the current term.
 */
void shutdownInterface(interface_t *interface);

/**
 * Binding the send message function to a C function call
 *
 * @param interface The interface struct that holds the PyObject*, whether it's initialized, and the current term.
 * @param msg The message to send
 */
void sendMessage(interface_t *interface, char *msg);

/**
 * Runs the python function for handling requests for some max number.  I would recommend using a number like 5
 * and incorporating into whatever server structure something that checks in every few terms to make sure everything is ok.
 *
 * @param interface The interface struct that holds the PyObject*, whether it's initialized, and the current term.
 * @param max_terms How many terms to handle requests for (this number is a maximum, the user can shutdown externally)
 */
void handleRequests(interface_t *interface, int max_terms);


#endif //HOMEAUTOMATION_SLACKINTERFACE_H
