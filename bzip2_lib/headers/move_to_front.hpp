#ifndef MOVE_TO_FRONT_HPP
#define MOVE_TO_FRONT_HPP

#include <iostream>
#include <iostream>
#include <vector>
#include <numeric>
#include <list>

struct move_to_front
{

    static void encode(std::istream& input, std::ostream& output);

    static void decode(std::istream& input, std::ostream& output);

    static const int CHARS_AMOUNT = 256;
};

#endif /* MOVE_TO_FRONT_HPP */
