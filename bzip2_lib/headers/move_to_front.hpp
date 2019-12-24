#ifndef MOVE_TO_FRONT_HPP
#define MOVE_TO_FRONT_HPP

#include <iostream>
#include <iostream>
#include <vector>
#include <numeric>
#include <list>

extern const int CHAR_AMOUNT;

struct move_to_front
{

    static void encode(std::istream& input, std::ostream& output);

    static void decode(std::istream& input, std::ostream& output);

};

#endif /* MOVE_TO_FRONT_HPP */
