set(CMAKE_CXX_STANDARD 11)

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
endif ()

include_directories(${PYTHON_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES_control
        include/py_module.hpp
        include/communicate.hpp
        include/resnet.hpp
        include/dataset.hpp
        src/communicate.cpp
        src/resnet.cpp
        src/dataset.cpp
        main.cpp)

add_executable(control ${SOURCE_FILES_control})
target_link_libraries(control dlib ${PYTHON_LIBRARY} cblas clapack)