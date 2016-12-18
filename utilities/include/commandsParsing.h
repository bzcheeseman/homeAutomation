//
// Created by Aman LaChapelle on 8/19/16.
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

#ifndef HOMEAUTOMATION_COMMANDSPARSING_H
#define HOMEAUTOMATION_COMMANDSPARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @file commandsParsing.h
 * @brief Gets the commands from the logfile and parses them into a char**.
 *
 * Functions for reading from the commands logfile and parsing them into a char**.  The struct command_t also holds
 * a char** for log entries and the number of commands/entries held within.
 */

/**
 * @struct command_t
 *
 * Holds:
 *  - An array of actions to be completed
 *  - The current term (the term of the last command in the actions array)
 *  - The log entries for all of those actions
 *  - The number of entries - could back out the start term from this number if needed.
 */
typedef struct {
  //!An array of actions to be performed
  char **actions;
  //!The current term (as of the end of the actions array)
  int current_term;
  //!The log entries to be committed for this slice of actions
  char **log_entries;
  //!The number of entries in this struct (length of char **actions and char **log_entries)
  int entries;
} command_t ;

/**
 * Reads the commands file and returns a struct that holds an array of commands, the last term entered, the
 * number of entries and an array of log entries.
 *
 * @param filename The name of the log file.  Will default to "logging/commands.log"
 * @return command_t struct.  @see command_t.
 */
command_t *getTermCommands(char *filename, int current_term);

/**
 * Discards the commands (assumes they've been used already and we're done with them!
 * Frees all memory associated with the pointer passed to it.
 *
 * @param commands Commands to be discarded
 */
void discardCommands(command_t *commands);

/**
 * Commits the commands to log - assuming that something happened with them
 *
 * @param commands Commands to be committed to the log.
 */
void commitToLog(command_t *commands, char *filename);

/**
 * Prints commands to stdout
 *
 * @param commands Commands to be printed
 */
void printCommands(command_t *commands);

#endif //HOMEAUTOMATION_COMMANDSPARSING_H
