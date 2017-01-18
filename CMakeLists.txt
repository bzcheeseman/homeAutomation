cmake_minimum_required(VERSION 3.0)
project(homeAutomation)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
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

if (APPLE)
    set(BREWED 1)
endif()

add_definitions(-DPYCPP_PY_HOME="${CMAKE_CURRENT_SOURCE_DIR}")
add_definitions(-DPYC_PY_HOME="${CMAKE_CURRENT_SOURCE_DIR}")

find_package(PythonInterp 2.7 REQUIRED)

add_definitions(-DPYCPP_WHICH_PYTHON="${PYTHON_EXECUTABLE}")
add_definitions(-DPYC_WHICH_PYTHON="${PYTHON_EXECUTABLE}")

find_package(PythonLibs 2.7 REQUIRED)

if (BREWED)
    set(PYTHON_LIBRARY "/usr/local/Frameworks/Python.framework/Versions/Current/lib/libpython2.7.dylib")
    set(PYTHON_INCLUDE_DIR "/usr/local/Frameworks/Python.framework/Headers")
endif ()

include_directories(${PYTHON_INCLUDE_DIR})

add_subdirectory(control)
add_subdirectory(hardware)
add_subdirectory(archive/device/ml)
add_subdirectory(archive/device/python)
add_subdirectory(archive/device/utilities)
add_subdirectory(archive/device/wordToVec)

set(SOURCE_FILES archive/main.c)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # make this work for the other targets too - and clean this shit up!

add_executable(homeAutomation ${SOURCE_FILES})
target_link_libraries(homeAutomation ha_python)