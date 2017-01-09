set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Ofast")

set(SOURCE_FILES_comms
        ../utilities/py_module.hpp
        include/communicate.hpp
        include/log.hpp
        src/communicate.cpp
        src/log.cpp
        main.cpp)

add_executable(comms ${SOURCE_FILES_comms})
target_link_libraries(comms dlib ${PYTHON_LIBRARY})