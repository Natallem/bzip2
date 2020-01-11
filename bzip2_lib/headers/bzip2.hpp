#ifndef BZIP_2_HPP
#define BZIP_2_HPP

#include <iostream>
#include <sstream>
#include <functional>

#include "bwt.hpp"
#include "move_to_front.hpp"
#include "huffman.hpp"

class bzip2 {

    using coding_function = std::function<void(std::istream &input, std::ostream &output)> const &;

public:

    static void encode(std::istream &input, std::ostream &output);

    static void decode(std::istream &input, std::ostream &output);

private:

    static void coder(std::istream &input,
                      std::ostream &output,
                      coding_function function_1,
                      coding_function function_2,
                      coding_function function_3);
};

#endif //BZIP_2_HPP