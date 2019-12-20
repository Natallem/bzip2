#ifndef MOVETOFRONT_HPP
#define MOVETOFRONT_HPP
#include <iostream>

extern const int CHAR_AMOUNT;

struct move_to_front
{

    // Этап кодирования
    static void encode(std::istream& input, std::ostream& output);

    // Этап декодирования
    static void decode(std::istream& input, std::ostream& output);

};
#endif //MOVETOFRONT_HPP
