//
// Created by natalem on 23.12.2019.
//

#ifndef BZIP2_WITH_TESTS_READER_32_BIT_HPP
#define BZIP2_WITH_TESTS_READER_32_BIT_HPP


#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iosfwd>

class reader_32_bit {
public:

    static void write_uint32_t(uint32_t number, std::ostream &output);

    static uint32_t read_uint32_t(std::istream &input);

};


#endif //BZIP2_WITH_TESTS_READER_32_BIT_HPP
