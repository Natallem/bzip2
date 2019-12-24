project(bzip2)

set(HEADER_FILES
        sources/circular_suffix_array.cpp
        sources/huffman.cpp
        sources/move_to_front.cpp
        sources/bwt.cpp
        sources/bzip2.cpp
        sources/reader_32_bit.cpp headers/reader_32_bit.hpp)

set(SOURCE_FILES
        headers/huffman.hpp
        headers/bwt.hpp
        headers/move_to_front.hpp
        headers/circular_suffix_array.hpp
        headers/bzip2.hpp
        sources/reader_32_bit.cpp headers/reader_32_bit.hpp)

add_library(bzip2_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})