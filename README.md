**homeAutomation**

The point of this project is to automate the home as much as possible with
some amount of NLP - purely context-based recognition.

So far the parsing functions are built and most of the background mechanics
are working as well.  Next steps will be to actually pass words to 
the SVM and see how well it does, as well as add kernel methods to 
improve classification.

PROJECT TODO:
* Kernel methods to get more than simply a linear SVM working
* Read the Brown corpus and use that for training data
* Pass the word data in a meaningful (context-based) way to the SVM
* Once the NeuralNetworks project gets going for real, explore using
    LSTM or RNN models instead of an SVM for context understanding
    
This code was written in C so as to be light on the processor - the idea 
being that it would be useful on an Arduino or other similar integrated chip processor.

It makes use of Python integration for Slack utilities and plotting SVM
results, since it's still untested I'm not sure if those will cause a significant
slowdown on a small chip.  My hope is that it won't simply because we 
aren't starting the Python interpreter.



Dependencies:
```
Python - matplotlib
Python - slacker
Doxygen - To rebuild documentation
Electron - To view documentation
Slack Bot token(!!!)
```

[slacker](https://github.com/os/slacker) has been quite useful - give them a look!

[matplotlib](http://matplotlib.org/) of course is a staple of plotting and scientific 
python users everywhere.

[Doxygen](http://www.stack.nl/~dimitri/doxygen/) is a wonderful utility for anyone writing
code for any reason (it works well in many languages, C and C++ happen to be two of them)

[electron](https://github.com/electron/electron/blob/master/docs/tutorial/quick-start.md) is
a super cool tool that I've begun to use to view my handy dandy Doxygen
documentation without having to put the pages up and host them somehow.  Definitely not an expert
but it's fun to play with.

To build and run the test program:
```
mkdir build && cd build
cmake ..
export SLACK_KEY=<your slack bot token>
./homeAutomation
```

A custom target was added to the CMakeLists.txt file (and thus to the makefile)
that allows you to do the following to run doxygen and make the documentation:
```
cd build
make docs
```

For the documentation, there are some cool features using Doxygen and Electron
where you can do the following:
```
cd homeAutomation/docs
electron .
```
which will show the documentation in its own little chromium window
provided you have [electron](https://github.com/electron/electron/blob/master/docs/tutorial/quick-start.md) installed.

