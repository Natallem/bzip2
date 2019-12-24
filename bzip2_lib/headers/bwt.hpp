#ifndef BWT_HPP
#define BWT_HPP

#include <iostream>

extern const int CHAR_AMOUNT;

struct BWT
{
    static void encode(std::istream &input, std::ostream &output);

    static void decode(std::istream &input, std::ostream &output);
};

#endif /* BWT_HPP */
