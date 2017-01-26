set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

include_directories(${PYTHON_INCLUDE_DIR})

set(messaging_SOURCE_FILES
        include/log.hpp
        include/message.hpp
        include/broker.hpp
        src/broker.cpp
        main.cpp)

add_executable(messaging ${messaging_SOURCE_FILES})
target_link_libraries(messaging zmq zmqpp sodium dlib)