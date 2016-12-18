set(BREWED 1)

find_package(PythonInterp 2.7 REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)

if (BREWED)
    set(PYTHON_LIBRARY "/usr/local/Frameworks/Python.framework/Versions/Current/lib/libpython2.7.dylib")
endif ()

include_directories(${PYTHON_INCLUDE_DIR})

set(SOURCE_FILES_python
        ha_python.h
        include/plotting.h
        include/slackInterface.h
        src/plotting.c
        src/slackInterface.c)

add_library(ha_python ${SOURCE_FILES_python})
target_link_libraries(ha_python ${PYTHON_LIBRARY})