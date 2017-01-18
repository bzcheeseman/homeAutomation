set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Ofast")

include_directories(${PYTHON_INCLUDE_DIR})

set(SOURCE_FILES_hardware
        ../python/py_module.hpp

        devices/include/device.hpp
        devices/include/compute_device.hpp
        devices/include/raspi.hpp

        devices/src/device.cpp
        devices/src/compute_device.cpp
        devices/src/raspi.cpp

        utilities/include/data_log.hpp
        utilities/src/data_log.cpp

        devices/devices.hpp

        main.cpp)

add_executable(hardware ${SOURCE_FILES_hardware})
target_link_libraries(hardware dlib ${PYTHON_LIBRARY})