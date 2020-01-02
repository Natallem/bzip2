#include "../headers/circular_suffix_array.hpp"

extern const int CHAR_AMOUNT = 256;

long long int circular_suffix_array::recount_classes(
        std::vector<std::pair<long long int, std::pair<long long int, long long int >>> &suffixes,
        long long int shift) {
    std::vector<long long int> class_of_suffix(suffixes.size());
    long long int current_class = 0;
    for (long long int i = 1; i < suffixes.size(); i++) {
        if (suffixes[i - 1].second != suffixes[i].second) ++current_class;
        class_of_suffix[suffixes[i].first] = current_class;
    }
    for (auto &triple : suffixes) {
        triple.second.second = class_of_suffix[triple.first];
        if (triple.first < shift) {
            triple.first = triple.first + suffixes.size() - shift;
        } else {
            triple.first -= shift;
        }
        triple.second.first = class_of_suffix[triple.first];
    }
    return current_class;
}

void circular_suffix_array::count_sort(
        std::vector<std::pair<long long int, std::pair<long long int, long long int>>> &suffixes,
        long long int max_class) {
    std::vector<long long int> class_counter(max_class + 1, 0);
    for (auto triple : suffixes) {
        ++class_counter[triple.second.first];
    }
    long sum = 0;
    for (long long int i = 0; i < class_counter.size(); i++) {
        long prev = sum;
        sum += class_counter[i];
        class_counter[i] = prev;
    }
    std::vector<std::pair<long long int, std::pair<long long int, long long int>>> new_suffixes(suffixes.size());

    for (auto triple : suffixes) {
        new_suffixes[class_counter[triple.second.first]++] = triple;
    }
    suffixes = new_suffixes;
}


circular_suffix_array::circular_suffix_array(const std::vector<uint8_t> &input)
        : indexes(input.size()) {
    std::vector<std::pair<long long int, std::pair<long long int, long long int>>> suffixes;
    for (long long int i = 0; i < input.size(); i++) {
        suffixes.push_back({i, {input[i], 0}});
    }
    count_sort(suffixes, 256);
    recount_classes(suffixes, 0);
    for (long long int i = 1; i <= input.size(); i = i << 1) {
        long long int max_class = recount_classes(suffixes, i);
        count_sort(suffixes, max_class);
    }
    for (long long int i = 0; i < suffixes.size(); i++) {
        indexes[i] = (suffixes[i].first);
    }
}

size_t circular_suffix_array::size() const {
    return indexes.size();
}

size_t circular_suffix_array::operator[](size_t n) const {
    return indexes[n];
}
