## homeAutomation

The point of this project is to automate the home as much as possible with
some amount of NLP - purely context-based recognition.

**PROJECT TODO:**
* word2vec implementation - MITIE
* OCR implementation
* control <=> action communication
* action ML methods - not just SVM

Given the complexity and computational intensity of the project, I've decided
to split the project into two parts, control and action, corresponding to a 
GPU-equipped computer and an integrated circuit board like an Arduino or 
Raspberry Pi with limited computing power.

Because of this restructuring, I'm working mostly (for now) on the parts of the
project that will be done on the control side, once this is done I will be able 
to keep working on the other part.

Since the Brown corpus is so unwieldy, I'm going to use [MITIE](https://github.com/mit-nlp/MITIE)
for the NLP component and extracting the word vector embeddings.

I've decided to use [dlib](http://dlib.net/ml.html) for the computer-side machine learning
components, so that's character OCR and training the word2vec model.

Once the word2vec model is trained, I'll send the matrix to the integrated chip and 
then perform the context recognition on the chip directly to avoid potential issues. 
The OCR will never work really well without a beefier computer, so I'll implement some
sort of protocol for sending the pictures around.

The action code was written in C so as to be light on processing and RAM, while control has no
such constraints. It makes use of Python so as to use `slacker` and `matplotlib` for my own 
convenience. To this end, I've included (and will use) my [Py_Cpp project](https://github.com/bzcheeseman/Py_Cpp)
which will be used for my own convenience wherever possible.

## Dependencies:
```
Python - matplotlib
Python - slacker
Slack Bot token(!!!)
Doxygen - To rebuild documentation
Electron - To view documentation
dlib (with or without CUDNN - the GPU will speed things up)
MITIE
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

[dlib](http://dlib.net/ml.html) is a fairly general purpose library with a fantastic support
for machine learning APIs in C and/or C++. It makes building and training deep neural networks
easy and fast.

[MITIE](https://github.com/mit-nlp/MITIE) is a library built on dlib that allows for fast 
feature extraction.

## Build/Run

To build and run the test program:
```
mkdir build && cd build
cmake .. && make
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

## Data

For training the various machine learning models, I use the following datasets:
* [Chars74K](http://www.ee.surrey.ac.uk/CVSSP/demos/chars74k/) for OCR
* [mnist](http://yann.lecun.com/exdb/mnist/) also for OCR
* [Brown Corpus](http://www.sls.hawaii.edu/bley-vroman/brown_nolines.txt) for 
context recognition and NLP - there may be a better solution for this one
