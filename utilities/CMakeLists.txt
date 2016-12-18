set(SOURCE_FILES_utilities
        utilities.h
        include/commandsParsing.h
        include/hashTable.h
        include/matrix.h
        include/vector.h
        src/commandsParsing.c
        src/hashTable.c
        src/matrix.c
        src/vector.c)

add_library(utilities ${SOURCE_FILES_utilities})