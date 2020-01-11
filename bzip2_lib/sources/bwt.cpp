#include <iostream>
#include <headers/move_to_front.hpp>
#include "../headers/bwt.hpp"
#include "../headers/circular_suffix_array.hpp"
#include "../headers/reader_32_bit.hpp"

void BWT::encode(std::istream &input, std::ostream &output)
{

    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
    circular_suffix_array suffixArray(input_chars);
    for (size_t i = 0; i < suffixArray.size(); i++) {
        if (!suffixArray[i]) {
            reader_32_bit::write_uint32_t(i,output);
            break;
        }
    }
    for (size_t i = 0; i < suffixArray.size(); i++) {
        size_t cur_index = suffixArray[i];
        if (!cur_index) output << input_chars[input_chars.size() - 1];
        else output << (uint8_t) input_chars[cur_index - 1];
    }
}

void BWT::decode(std::istream &input, std::ostream &output)
{
    if (input.rdbuf()->in_avail()==0) {
        output << "";
        return;
    }
    uint32_t number = reader_32_bit::read_uint32_t(input);
    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
    std::vector<size_t> chars_counter(move_to_front::CHARS_AMOUNT, 0);
    for (size_t i = 0; i < input_chars.size(); i++) {
        ++chars_counter[input_chars[i]];
    }
    size_t sum = 0;
    for (size_t i = 0; i < chars_counter.size(); i++) {
        sum += chars_counter[i];
        chars_counter[i] = sum - chars_counter[i];
    }
    std::vector<size_t> reverse_indexes(input_chars.size());
    for (size_t i = 0; i < input_chars.size(); i++) {
        reverse_indexes[chars_counter[input_chars[i]]] = i;
        ++chars_counter[input_chars[i]];
    }
    size_t j = reverse_indexes[number];
    for (size_t i = 0; i < input_chars.size(); i++) {
        output << input_chars[j];
        j = reverse_indexes[j];
    }
}



