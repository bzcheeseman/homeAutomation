//
// Created by Aman LaChapelle on 8/30/16.
//

#include "../include/plotting.h"

void plotData(vector_t *xdata, vector_t *ydata) { //perform xdecref's etc.

  assert(xdata->len == ydata->len);

  setenv("PYTHONPATH", "/Users/Aman/code/homeAutomation", 1);

  //Init interpreter
  Py_SetProgramName("/usr/local/bin/python");
  Py_Initialize();

  PyObject *plotterName = PyString_FromString("scripts.plotting");

  PyObject *plotter = PyImport_Import(plotterName); //import plotter
  Py_XDECREF(plotterName); //decref the name

  if (PyErr_Occurred()){
    PyErr_Print();
  }

  if (!plotter){
    Py_XDECREF(plotter);

    Py_Finalize();

    return;
  }

  else{

    PyObject *plotdata = PyObject_GetAttrString(plotter, "plot");
    Py_XDECREF(plotter);
    PyObject *x = PyList_New(xdata->len);
    PyObject *y = PyList_New(ydata->len);

    int end = xdata->len;
    for (int i = 0; i < end; i++){
      PyList_SetItem(x, i, PyFloat_FromDouble(xdata->data[i]));
      PyList_SetItem(y, i, PyFloat_FromDouble(ydata->data[i]));
    }

    PyObject *args = PyTuple_Pack(2, x, y);
    PyObject *result = PyObject_CallObject(plotdata, args);

  }
}

void plotLine(double m, double b) {
  setenv("PYTHONPATH", "/Users/Aman/code/homeAutomation", 1);

  //Init interpreter
  Py_SetProgramName("/usr/local/bin/python");
  Py_Initialize();

  PyObject *plotterName = PyString_FromString("scripts.plotting");

  PyObject *plotter = PyImport_Import(plotterName); //import plotter
  Py_XDECREF(plotterName); //decref the name

  if (PyErr_Occurred()){
    PyErr_Print();
  }

  if (!plotter){
    Py_XDECREF(plotter);

    Py_Finalize();

    return;
  }

  else{

    PyObject *plotline = PyObject_GetAttrString(plotter, "plotLine");
    Py_XDECREF(plotter);

    PyObject *args = PyTuple_Pack(2, PyFloat_FromDouble(m), PyFloat_FromDouble(b));
    PyObject *result = PyObject_CallObject(plotline, args);

  }
}

void plotDataLine(vector_t *xdata, vector_t *ydata, double m, double b) {
  setenv("PYTHONPATH", "/Users/Aman/code/homeAutomation", 1);

  //Init interpreter
  Py_SetProgramName("/usr/local/bin/python");
  Py_Initialize();

  PyObject *plotterName = PyString_FromString("scripts.plotting");

  PyObject *plotter = PyImport_Import(plotterName); //import plotter
  Py_XDECREF(plotterName); //decref the name

  if (PyErr_Occurred()){
    PyErr_Print();
  }

  if (!plotter){
    Py_XDECREF(plotter);

    Py_Finalize();

    return;
  }

  else{

    PyObject *plotdataline = PyObject_GetAttrString(plotter, "plotDataLine");
    Py_XDECREF(plotter);

    PyObject *x = PyList_New(xdata->len);
    PyObject *y = PyList_New(ydata->len);

    int end = xdata->len;
    for (int i = 0; i < end; i++){
      PyList_SetItem(x, i, PyFloat_FromDouble(xdata->data[i]));
      PyList_SetItem(y, i, PyFloat_FromDouble(ydata->data[i]));
    }

    PyObject *args = PyTuple_Pack(4, x, y, PyFloat_FromDouble(m), PyFloat_FromDouble(b));

    PyObject *result = PyObject_CallObject(plotdataline, args);

  }
}

void plotArray(vector_t *array, int dim) {
  setenv("PYTHONPATH", "/Users/Aman/code/homeAutomation", 1);

  //Init interpreter
  Py_SetProgramName("/usr/local/bin/ipython");
  Py_Initialize();

  PyObject *plotterName = PyString_FromString("scripts.plotting");

  PyObject *plotter = PyImport_Import(plotterName); //import plotter
  Py_XDECREF(plotterName); //decref the name

  if (PyErr_Occurred()){
    PyErr_Print();
  }

  if (!plotter){
    Py_XDECREF(plotter);

    Py_Finalize();

    return;
  }

  else{

    PyObject *plotarray = PyObject_GetAttrString(plotter, "plot2DArray");
    Py_XDECREF(plotter);

    PyObject *x = PyList_New(array->len);

    size_t end = array->len;
    for (size_t i = 0; i < end; i++){
      PyList_SetItem(x, i, PyFloat_FromDouble(array->data[i]));
    }

    PyObject *args = PyTuple_Pack(2, x, PyFloat_FromDouble((double)dim));

    PyObject *result = PyObject_CallObject(plotarray, args);

    if (PyErr_Occurred()){
      PyErr_Print();
    }

  }
}
