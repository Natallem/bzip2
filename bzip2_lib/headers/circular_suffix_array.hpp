#ifndef CIRCULAR_SUFFIX_ARRAY_HPP
#define CIRCULAR_SUFFIX_ARRAY_HPP

#include <string>
#include <vector>

extern const int CHAR_AMOUNT;


class circular_suffix_array {

public:

    explicit circular_suffix_array(const std::vector<uint8_t> &input);

    size_t size() const;

    size_t operator[](size_t n) const;

private:

    std::vector<size_t> indexes;

    void count_sort(std::vector<std::pair<long long int, std::pair<long long int, long long int>>> &suffixes,
                    long long int max_class);

    long long int
    recount_classes(std::vector<std::pair<long long int, std::pair<long long int, long long int>>> &suffixes,
                    long long int shift);

};

#endif /* CIRCULAR_SUFFIX_ARRAY_HPP */
