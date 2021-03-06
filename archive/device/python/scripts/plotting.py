'''
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
'''

# TODO: Comment this code

import matplotlib
matplotlib.use("Qt4Agg")

from matplotlib import pyplot as plt
import numpy as np


def plot(xdata, ydata):
    plt.figure(figsize=(10, 10))
    plt.plot(xdata, ydata, 'o')
    plt.show()

def plotLine(m, b):
    plt.figure(figsize=(10, 10))
    def line(x, m, b):
        return m*x + b
    plt.plot(np.arange(0, 2), line(np.arange(0, 2), m, b))
    plt.show()

def plotSave(filename, xdata, ydata):
    plt.figure(figsize=(10, 10))
    plt.plot(xdata, ydata, 'o')
    plt.savefig(filename)

def plotDataLine(xdata, ydata, m, b):
    plt.figure(figsize=(10, 10))
    def line(x, m, b):
        return m*x + b
    ax = plt.gca()
    ax.set_xlim([0, 1])
    ax.set_ylim([0, 1])
    plt.plot(np.linspace(0., 1., 10), line(np.linspace(0., 1., 10), m, b))
    # plt.plot(np.linspace(0., 1., 10), line(np.linspace(0., 1., 10), -1/m, 0))
    plt.plot(xdata, ydata, 'o')
    plt.show()

def plot2DArray(flattenedArray, dim):
    array = np.array(flattenedArray, dtype=float)
    dim = int(dim)
    array = np.reshape(array, (dim, dim))
    plt.figure(figsize=(10, 10))
    # oneax = np.arange(0., 1., 1./dim)
    # x, y = np.meshgrid(oneax, oneax)
    # plt.contour(x, y, array)
    plt.pcolor(np.linspace(0, 1, dim), np.linspace(0, 1, dim), array.T)
    # plt.plot([0.5], [0.5], 'o')
    plt.colorbar()
    plt.show()