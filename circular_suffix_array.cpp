#include "circular_suffix_array.hpp"

const int CHAR_AMOUNT = 256;

CircularSuffixArray::CircularSuffixArray(const std::string &input)
        : indexes(input.size()) {
    const size_t SYMBOLS = CHAR_AMOUNT;
    size_t length = input.size();
    std::vector<size_t> classes(length);

    std::vector<size_t> counter(SYMBOLS, 0);
    for (char ch : input)
        ++counter[ch];

    for (int i = 1; i < SYMBOLS; i++)
        counter[i] += counter[i - 1];

    for (long int i = length - 1; i >= 0; i--)
        indexes[--counter[input[i]]] = i;

    classes[indexes[0]] = 0;
    int class_amount = 1;
    for (int i = 1; i < length; ++i) {
        if (input[indexes[i]] != input[indexes[i - 1]]) ++class_amount;
        classes[indexes[i]] = class_amount - 1;
    }

    std::vector<long int> sorted_second_part(length);
    std::vector<size_t> new_classes(length);

    for (size_t pow = 0; (1 << pow) < length; ++pow) {
        size_t cur_pow = 1 << pow;
        std::fill(counter.begin(), counter.begin() + class_amount, 0);

        for (size_t i = 0; i < length; ++i) {
            sorted_second_part[i] = indexes[i] - cur_pow;
            if (sorted_second_part[i] < 0) sorted_second_part[i] += length;
        }

        for (size_t i = 0; i < length; ++i)
            ++counter[classes[sorted_second_part[i]]];

        for (size_t i = 1; i < class_amount; ++i)
            counter[i] += counter[i - 1];

        for (long int i = length - 1; i >= 0; --i)
            indexes[--counter[classes[sorted_second_part[i]]]] = sorted_second_part[i];

        new_classes[indexes[0]] = 0;
        class_amount = 1;

        for (size_t i = 1; i < length; ++i) {
            size_t part_1 = (indexes[i] + cur_pow) % length;
            size_t part_2 = (indexes[i - 1] + cur_pow) % length;
            if (classes[indexes[i]] != classes[indexes[i - 1]] || classes[part_1] != classes[part_2])
                ++class_amount;
            new_classes[indexes[i]] = class_amount - 1;
        }

        classes = new_classes;
    }
}

size_t CircularSuffixArray::size() const {
    return indexes.size();
}

size_t CircularSuffixArray::operator[](size_t n) const {
    return indexes[n];
}