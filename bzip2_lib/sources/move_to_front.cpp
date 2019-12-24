
#include "../headers/move_to_front.hpp"

void move_to_front::encode(std::istream &input, std::ostream &output)
{
    std::list<uint8_t> symbols(CHAR_AMOUNT);
    std::iota(symbols.begin(), symbols.end(), 0);
    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
    for (uint8_t cur_char : input_chars) {
        auto symbols_itr = symbols.begin();
        uint8_t counter = 0;
        for (; cur_char != *symbols_itr && symbols_itr != symbols.end(); ++counter, ++symbols_itr);
        output << counter;
        symbols.emplace_front(*symbols_itr);
        symbols.erase(symbols_itr);
    }
}

void move_to_front::decode(std::istream &input, std::ostream &output)
{
    std::vector<uint8_t> symbols(CHAR_AMOUNT);
    std::iota(symbols.begin(), symbols.end(), 0);
    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
    for (uint8_t cur_char : input_chars) {
        output << symbols[cur_char];
        std::swap(symbols[cur_char], symbols[0]);
        for (uint8_t i = cur_char; i > 1; --i) {
            std::swap(symbols[i], symbols[i - 1]);
        }
    }
}
