set(SOURCE_FILES_wordtovec
        wordToVec.h
        include/ngrams.h
        include/word_to_vec.h
        src/ngrams.c
        src/word_to_vec.c)

add_library(wordToVec ${SOURCE_FILES_wordtovec})