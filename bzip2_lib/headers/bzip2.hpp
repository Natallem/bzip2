#ifndef BZIP_2_HPP
#define BZIP_2_HPP

#include <iostream>
#include <sstream>
#include <functional>

#include "bwt.hpp"
#include "move_to_front.hpp"
#include "huffman.hpp"

class bzip2 {

public:

    static void encode(std::istream &input, std::ostream &output);

    static void decode(std::istream &input, std::ostream &output);

private:

    static void coder(std::istream &input,
                      std::ostream &output,
                      std::function<void(std::istream &input, std::ostream &output)> const &function_1,
                      std::function<void(std::istream &input, std::ostream &output)> const &function_2,
                      std::function<void(std::istream &input, std::ostream &output)> const &function_3);
};

#endif //BZIP_2_HPP