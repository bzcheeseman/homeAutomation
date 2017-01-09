//
// Created by Aman LaChapelle on 8/30/16.
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

#ifndef HOMEAUTOMATION_PLOTTING_H
#define HOMEAUTOMATION_PLOTTING_H

#include <Python.h>
#include <stdio.h>
#include <unistd.h>
#include "../../utilities/include/vector.h"

/*
 * TODO: Documentation for this .h file
 * TODO: Clean up python references
 */

void plotData(vector_t *xdata, vector_t *ydata);

void plotLine(double m, double b);

void plotDataLine(vector_t *xdata, vector_t *ydata, double m, double b);

void plotArray(vector_t *array, int dim);

void plotSave(char *filename, vector_t *xdata, vector_t *ydata);

#endif //HOMEAUTOMATION_PLOTTING_H
