#include "headers/reader_32_bit.hpp"

void reader_32_bit::write_uint32_t(uint32_t number, std::ostream &output) {
    uint64_t mask1 = (1 << 8) - 1;
    uint32_t mask = mask1 << (3 * 8);
    for (int i = 3; i >= 0; i--) {
        uint32_t current_bit = (mask & number) >> (i * 8);
        output << (uint8_t) current_bit;
        mask >>= 8;
    }
}

uint32_t reader_32_bit::read_uint32_t(std::istream &input) {
    std::vector<uint8_t> number_in_uint8_t(4);
    uint32_t result = 0;
    std::copy_n(std::istreambuf_iterator<char>{input}, 5, begin(number_in_uint8_t));
    for (int i = 3; i >= 0; i--) {
        result |= (number_in_uint8_t[3 - i] << (8 * i));
    }
    return result;
}
