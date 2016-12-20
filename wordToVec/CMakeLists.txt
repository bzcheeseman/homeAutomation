set(SOURCE_FILES_wordtovec
        wordToVec.h
        include/ngrams.h
        include/word_to_vec.h
        src/ngrams.c
        src/word_to_vec.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

add_library(wordToVec ${SOURCE_FILES_wordtovec})