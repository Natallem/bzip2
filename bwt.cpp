#include <iostream>
#include "bwt.hpp"
#include "circular_suffix_array.hpp"

void BWT::transform(std::istream &input, std::ostream &output)
{
    std::string string_(std::istreambuf_iterator<char>(input), {});
    CircularSuffixArray suffixArray(string_);

    for (size_t i = 0; i < suffixArray.size(); i++) {
        if (suffixArray[i] == 0) {
            output << i << "\n";
            break;
        }
    }

    for (size_t i = 0; i < suffixArray.size(); i++) {
        size_t cur_index = suffixArray[i];
        if (cur_index == 0) output << string_[string_.size() - 1];
        else output << string_[cur_index - 1];
    }
}

void BWT::inverseTransform(std::istream &input, std::ostream &output)
{
    char _char;
    size_t number;
    char ch;
    input >> number;
    input.get(_char);
//    std::string string_;
//    while (input.get(_char)) {
//        string_ += _char;
//    }
    std::string string_(std::istreambuf_iterator<char>(input), {});
//    std::cout << string_;

    std::vector<size_t> chars_counter(CHAR_AMOUNT, 0);
    for (size_t i = 0; i < string_.length(); i++) {
        ++chars_counter[string_[i]];
    }
    size_t sum = 0;
    for (int i = 0; i < CHAR_AMOUNT; i++) {
        sum += chars_counter[i];
        chars_counter[i] = sum - chars_counter[i];
    }
    std::vector<size_t> reverse_indexes(string_.size());
    for (size_t i = 0; i < string_.size(); i++) {
        reverse_indexes[chars_counter[string_[i]]] = i;
        ++chars_counter[string_[i]];
    }
    size_t j = reverse_indexes[number];
    for (size_t i = 0; i < string_.size(); i++) {
        output << string_[j];
        j = reverse_indexes[j];
    }
}

