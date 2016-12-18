set(SOURCE_FILES_wordtovec
        wordToVec.h
        include/bigrams.h
        include/word_to_vec.h
        src/bigrams.c
        src/word_to_vec.c)

add_library(wordToVec ${SOURCE_FILES_wordtovec})