//
// Created by Aman LaChapelle on 8/19/16.
//

#include "../include/commandsParsing.h"

command_t *getTermCommands(char *filename, int current_term){

  if (filename == NULL){
    filename = "/Users/Aman/code/homeAutomation/logging/commands.log";
  }

  command_t *cmds = malloc(sizeof(command_t));

  cmds->current_term = current_term; //The term won't change over this whole process!

  cmds->actions = malloc(sizeof(char*));
  cmds->actions[0] = NULL;
  FILE *cmdFile = fopen(filename, "r+");
  int numLines = 0;
  size_t len = 0;
  //Read the file line by line
  while (getline(&(cmds->actions[numLines]), &len, cmdFile) != -1){
    numLines++;
    cmds->actions = realloc(cmds->actions, sizeof(char*)*(numLines + 1));
    cmds->actions[numLines] = NULL;
  }
  //Record the number of lines in this struct
  cmds->entries = numLines;

  //Set up the log entries
  cmds->log_entries = malloc(sizeof(char*) * numLines);
  char recv[64] = "Received: ";

  //Build the log entries
  for (int i = 0; i < numLines; i++){
    strncat(recv, cmds->actions[i], strlen(cmds->actions[i]));
    cmds->log_entries[i] = calloc(sizeof(char), (strlen(recv) + 1));
    strncpy(cmds->log_entries[i], recv, strlen(recv));
  }
  //Clean up
  fclose(cmdFile);

  return cmds;
}

void discardCommands(command_t *commands){
  int end = commands->entries;
  for (int i = 0; i < end; i++){
    free(commands->actions[i]);
    free(commands->log_entries[i]);
  }
  free(commands->actions);
  free(commands->log_entries);

  free(commands);
}

void commitToLog(command_t *commands, char *filename){

  if (filename == NULL){
    filename = "/Users/Aman/code/homeAutomation/logging/commandsParsing.log";
  }
  FILE *log = fopen(filename, "w+");

  if (!log){
    printf("Couldn't open the file!");
    return;
  }

  int end = commands->entries;
  for (int i = 0; i < end; i++){
    fprintf(log, "Term: %d - ", commands->current_term);
    fprintf(log, commands->log_entries[i]);
  }

  fclose(log);

  return;
}

void printCommands(command_t *commands){
  const int end = commands->entries;
  for (int i = 0; i < end; i++){
    printf(commands->actions[i]);
    printf(commands->log_entries[i]);
  }
}