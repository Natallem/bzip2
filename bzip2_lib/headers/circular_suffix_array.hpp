#ifndef CIRCULAR_SUFFIX_ARRAY_HPP
#define CIRCULAR_SUFFIX_ARRAY_HPP

#include <string>
#include <vector>

class circular_suffix_array {

public:

    explicit circular_suffix_array(const std::vector<uint8_t> &input);

    size_t size() const;

    size_t operator[](size_t n) const;

private:

    std::vector<size_t> indexes;

    void count_sort(std::vector<std::pair<int64_t, std::pair<int64_t, int64_t >>> &suffixes,
                    int64_t max_class);

    int64_t
    recount_classes(std::vector<std::pair<int64_t, std::pair<int64_t, int64_t> >> &suffixes,
                    int64_t shift);

};

#endif /* CIRCULAR_SUFFIX_ARRAY_HPP */
