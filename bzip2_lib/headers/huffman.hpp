#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

#include "reader_32_bit.hpp"

/* FOR MORE INFORMATION ABOUT ENCODING/DECODING DATA REPRESENTATION CHECK README.MD */

struct huffman {
public:

    static void encode(std::istream &input, std::ostream &output);

    static void decode(std::istream &input, std::ostream &output);

private:

    struct Tree {

        Tree(unsigned long int frequency, short ch = -1);

        Tree(Tree *left, Tree *right);

        ~Tree();

        unsigned long int frequency = 0;
        short ch = -1;
        Tree *left = nullptr;
        Tree *right = nullptr;
    };

    static std::vector<unsigned long int> count_chars_frequency(const std::string &input_chars);

    static Tree *createHuffmanEncodeTree(const std::vector<unsigned long int> &counter);

    struct Encode_table {

        explicit Encode_table(Tree *root);

        void dfs(Tree *cur_node, std::vector<bool> &cur_code);

        void create_encode_table();

        void write_encode_table(std::ostream &output);

        void encode_data(const std::string &data, std::ostream &output);

        struct bit_data_buffer {
            void add_bit(bool bit);

            unsigned getSize() const;

            void flush_byte();

            void add_byte();

            void write_result(std::ostream &output);

        private:
            std::vector<uint8_t> result;
            uint8_t byte = 0;
            short shift = 0;

        };

        Tree *root;
        std::string chars;
        bit_data_buffer compressed_huffman_tree;
        std::unordered_map<char, std::vector<bool>> codes;
    };

    struct Decode_table {

        Decode_table(std::istream &input);//create Huffman tree

        void decode(std::istream &input, std::ostream &output);

        Tree *dfs();

        bool read_bit(int index);

    private:
        std::vector<uint8_t> bytes;
        int ptr_huffman = 0;
        int ptr_alphabet = 0;
        Tree *root;
        std::vector<uint8_t> alphabet;
    };

};
#endif  /* HUFFMAN_HPP */
