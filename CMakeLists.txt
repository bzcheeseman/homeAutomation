cmake_minimum_required(VERSION 3.5)
project(homeAutomation)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -fopenmp")
#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-6)
set(CMAKE_C_COMPILER /usr/local/bin/gcc-6)

find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile @ONLY)
    add_custom_target(docs
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
endif(DOXYGEN_FOUND)

set(PYTHON_INCLUDE_DIR /usr/local/Frameworks/Python.framework/Headers)
set(PYTHON_LIBRARY /usr/local/Frameworks/Python.framework)

include_directories(${PYTHON_INCLUDE_DIR})

set(SOURCE_FILES src/slackInterface.c include/slackInterface.h main.c src/commandsParsing.c include/commandsParsing.h src/classificationMachine.c include/classificationMachine.h src/vectorOps.c include/vectorOps.h src/hashTable.c include/hashTable.h src/plotting.c include/plotting.h)

add_executable(homeAutomation ${SOURCE_FILES})
target_link_libraries(homeAutomation ${PYTHON_LIBRARY})