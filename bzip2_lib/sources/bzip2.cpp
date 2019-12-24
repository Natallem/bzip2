#include "headers/bzip2.hpp"

void bzip2::encode(std::istream &input, std::ostream &output) {
    coder(input, output, BWT::encode, move_to_front::encode, huffman::encode);
}

void bzip2::decode(std::istream &input, std::ostream &output) {
    coder(input, output, huffman::decode, move_to_front::decode, BWT::decode);
}

void bzip2::coder(std::istream &input, std::ostream &output,
                  std::function<void(std::istream &input, std::ostream &output)> const &function_1,
                  std::function<void(std::istream &input, std::ostream &output)> const &function_2,
                  std::function<void(std::istream &input, std::ostream &output)> const &function_3) {

    std::stringstream encoding_data_1;
    std::stringstream encoding_data_2;
    function_1(input, encoding_data_1);
    function_2(encoding_data_1, encoding_data_2);
    function_3(encoding_data_2, output);

}

