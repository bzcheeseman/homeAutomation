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

add_subdirectory(ml)
add_subdirectory(python)
add_subdirectory(utilities)
add_subdirectory(wordToVec)

include_directories(${PYTHON_INCLUDE_DIR})

set(SOURCE_FILES main.c)

add_executable(homeAutomation ${SOURCE_FILES})
target_link_libraries(homeAutomation ml ha_python utilities wordToVec)