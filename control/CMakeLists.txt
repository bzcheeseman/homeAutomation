set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Ofast")

set(SOURCE_FILES_control
        ../utilities/py_module.hpp
        include/ocrnet.hpp
        include/dataset.hpp
        src/ocrnet.cpp
        src/dataset.cpp
        main.cpp)

add_executable(control ${SOURCE_FILES_control})
target_link_libraries(control dlib ${PYTHON_LIBRARY} cblas clapack)