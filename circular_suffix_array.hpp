#ifndef CIRCULAR_SUFFIC_ARRAY_CPP_HPP
#define CIRCULAR_SUFFIC_ARRAY_CPP_HPP
#include <string>
#include "vector"
extern const int CHAR_AMOUNT;

struct CircularSuffixArray
{
    // Ctor
    explicit CircularSuffixArray(const std::string &input);

    // Returns size of the input string
    size_t size() const;

    // Returns index of suffix from original array
    size_t operator[](size_t n) const;

private:
    std::vector<size_t> indexes;
};

#endif //CIRCULAR_SUFFIC_ARRAY_CPP_HPP
