//
// Created by Aman LaChapelle on 8/14/16.
//

#include "../include/slackInterface.h"



void importInterface(interface_t *interface){

  setenv("PYTHONPATH", "/Users/Aman/code/homeAutomation/archive/device/python", 1); //fix this too
  char *token = getenv("SLACK_KEY");

  //Initialize the Interpreter
  Py_SetProgramName("/usr/local/bin/python");
  Py_Initialize();

  PyObject *pName = PyString_FromString("scripts.slack");

  PyObject *pModule = PyImport_Import(pName);
  Py_XDECREF(pName);

  if (PyErr_Occurred()){
    PyErr_Print();
  }

  if (!pModule){
    Py_XDECREF(pModule);

    Py_Finalize();

    return;
  }
  else{
    PyObject *pDict = PyModule_GetDict(pModule);

    PyObject *pClass = PyDict_GetItemString(pDict, "SlackServer");
    Py_XDECREF(pModule);

    PyObject *pInstance = NULL;
    if (pClass && PyCallable_Check(pClass)){
      PyObject *pArgs = PyTuple_New(1);
      PyTuple_SetItem(pArgs, 0, PyString_FromString(token));

      pInstance = PyObject_CallObject(pClass, pArgs);
    }

    interface->initialized = 1;
    if (pInstance != NULL){
      interface->interface = pInstance;
    }
    interface->current_term = 0;

    Py_XDECREF(pDict);
    Py_XDECREF(pClass);
  }
}

void shutdownInterface(interface_t *interface){
  Py_Finalize();
}

void sendMessage(interface_t *interface, char *msg){

  if (interface->initialized == 1){
    PyObject_CallMethod(interface->interface, "send", "(s)", msg);
  }
  else{
    fprintf(stderr, "Interface not initialized!");
  }

  return;

}

void handleRequests(interface_t *interface, int max_terms){

  if (interface->initialized == 1){

    int start_term = interface->current_term;
    int i = start_term;

    double buf1 = PyFloat_AsDouble(PyObject_CallMethod(interface->interface, "handle", NULL));
    double buf2 = 0.0;

    while (i < (max_terms + start_term) && buf2 != -1.0){ //The handle method returns -1.0 when the user exits
      PyObject *retval = PyObject_CallMethod(interface->interface, "handle", NULL);
      buf2 = PyFloat_AsDouble(retval);
      if (buf2 != buf1){ //Since it runs continuously, need to check if the timestamps changed to determine term increments
        i++;
        buf1 = PyFloat_AsDouble(retval);
      }

      Py_XDECREF(retval);
    }

    interface->current_term = i; //Set the current term to whatever the last term was

    fprintf(stderr, "Last term including shutdown: %d\n", interface->current_term);

  }
  else{

    fprintf(stderr, "Interface not initialized!");

  }

  return;

}