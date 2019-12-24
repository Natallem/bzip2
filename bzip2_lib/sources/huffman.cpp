#include "../headers/huffman.hpp"
#include "../headers/reader_32_bit.hpp"

extern const int CHAR_AMOUNT;

void huffman::encode(std::istream &input, std::ostream &output) {
    std::string data(std::istreambuf_iterator<char>(input), {});
    std::vector<unsigned long int> chars_frequency = count_chars_frequency(data);
    Tree *huffmanTree = createHuffmanEncodeTree(chars_frequency);
    Encode_table table(huffmanTree);
    table.create_encode_table();
    table.write_encode_table(output);
    table.encode_data(data, output);
    delete huffmanTree;
}

void huffman::decode(std::istream &input, std::ostream &output) {
    Decode_table table(input);//create Huffman decode tree
    table.decode(input, output);
}


std::vector<unsigned long int> huffman::count_chars_frequency(const std::string &input_chars) {
    std::vector<unsigned long int> chars_frequency(256, 0);
    for (char ch : input_chars) {
        int s = ch + 128;
        ++chars_frequency[ch + 128];
    }
    return chars_frequency;
}

huffman::Tree *huffman::createHuffmanEncodeTree(const std::vector<unsigned long int> &counter) {

    auto comp = [](Tree *x, Tree *y) {
        return x->frequency < y->frequency;
    };

    auto queue = std::multiset<Tree *, decltype(comp)>(comp);

    for (int i = 0; i < counter.size(); i++) {
        if (counter[i] > 0) {
            Tree *tr = new Tree(counter[i], i);
            queue.insert(tr);
        }
    }

    while (queue.size() > 1) {
        Tree *first = *queue.begin();
        queue.erase(queue.begin());
        Tree *second = *queue.begin();
        queue.erase(queue.begin());
        queue.insert(new Tree(first, second));
    }

    return *queue.begin();
};

huffman::Encode_table::Encode_table(Tree *root) : root(root) {}

void huffman::Encode_table::dfs(Tree *cur_node, std::vector<bool> &cur_code) {
    if (cur_node == nullptr) return;
    if (cur_node->left == nullptr) {//invariant: if left=nullptr then right=nullptr
        char current_char = cur_node->ch - 128;
        chars += current_char;
        codes.insert({current_char, cur_code});
        return;
    }

    compressed_huffman_tree.add_bit(0);
    cur_code.push_back(0);
    dfs(cur_node->left, cur_code);
    cur_code.pop_back();

    cur_code.push_back(1);
    compressed_huffman_tree.add_bit(1);
    dfs(cur_node->right, cur_code);
    cur_code.pop_back();
}

void huffman::Encode_table::create_encode_table() {
    std::vector<bool> cur_code;
    dfs(root, cur_code);
    if (chars.size() == 1) {
        codes[chars[0]] = {0};
        compressed_huffman_tree.add_bit(1);
    }
    compressed_huffman_tree.flush_byte();
}

void huffman::Encode_table::write_encode_table(std::ostream &output) {
    reader_32_bit::write_uint32_t(chars.length(),output);
    std::copy(chars.begin(), chars.end(), std::ostreambuf_iterator<char>(output));
    output << (uint8_t) compressed_huffman_tree.getSize();
    compressed_huffman_tree.write_result(output);
}

void huffman::Encode_table::encode_data(const std::string &data, std::ostream &output) {
    reader_32_bit::write_uint32_t((uint32_t) data.size(), output);
    uint8_t cur_num = 0;
    int cur_index = 0;
    for (char ch : data) {
        for (bool bit : codes[ch]) {
            cur_num <<= 1;
            cur_num |= bit;
            ++cur_index;
            if (cur_index == 8) {
                output << cur_num;
                cur_index = cur_num = 0;
            }
        }
    }
    if (cur_index != 0) {
        cur_num <<= (8 - cur_index);
        output << cur_num;
    }
}

void huffman::Encode_table::bit_data_buffer::add_bit(bool bit) {
    byte <<= 1;
    byte |= bit;
    if (++shift == 8) {
        add_byte();
    }
}

unsigned huffman::Encode_table::bit_data_buffer::getSize() const {
    return result.size();
}

void huffman::Encode_table::bit_data_buffer::flush_byte() {
    if (shift == 0) return;
    while (shift++ < 8) {
        byte <<= 1;
    }
    add_byte();
}

void huffman::Encode_table::bit_data_buffer::add_byte() {
    result.push_back(byte);
    shift = byte = 0;
}

void huffman::Encode_table::bit_data_buffer::write_result(std::ostream &output) {
    std::copy(result.begin(), result.end(), std::ostreambuf_iterator<char>(output));
}


huffman::Decode_table::Decode_table(std::istream &input) { //create Huffman tree
    uint32_t chars_amount =  reader_32_bit::read_uint32_t(input);
    if (chars_amount == 0) {
        root = nullptr;
        return;
    }
    alphabet.resize(chars_amount);
    std::copy_n(std::istreambuf_iterator<char>{input}, chars_amount + 1, begin(alphabet));
    uint8_t amount_bytes_huffman = input.get();
    bytes.resize(amount_bytes_huffman);
    std::copy_n(std::istreambuf_iterator<char>{input}, amount_bytes_huffman + 1, begin(bytes));
    root = dfs();
}

void huffman::Decode_table::decode(std::istream &input, std::ostream &output) {
    if (root == nullptr) return;
    uint32_t encoded_chars_amount = reader_32_bit::read_uint32_t(input);
    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                     std::istreambuf_iterator<char>());
    Tree *cur_node = root;
    uint32_t decoded_chars_counter = 0;
    for (auto byte : input_chars) {
        for (int i = (1 << 7); i > 0; i >>= 1) {
            if (cur_node->left != nullptr) {
                if (i & byte) {
                    cur_node = cur_node->right;
                } else {
                    cur_node = cur_node->left;
                }
            }
            if (cur_node->left == nullptr) {
                output << (char) cur_node->ch;
                if (++decoded_chars_counter == encoded_chars_amount) return;
                cur_node = root;
            }
        }
    }
}

huffman::Tree *huffman::Decode_table::dfs() {
    bool b = (ptr_alphabet == alphabet.size() - 1) ? true : read_bit(ptr_huffman++);
    if (b) {
        char ch = alphabet[ptr_alphabet++];
        Tree *tr = new Tree(0, alphabet.at(ptr_alphabet - 1));
        return tr;
    } else {
        Tree *node = new Tree(0, -1);
        node->left = dfs();
        node->right = dfs();
        return node;
    }
};

bool huffman::Decode_table::read_bit(int index) {
    int ptr_byte = (index) / 8;
    int ptr_bit = (index) % 8;
    return (bytes[ptr_byte] & (1 << (7 - ptr_bit)));
}


huffman::Tree::Tree(unsigned long int frequency, short ch) : ch(ch), frequency(frequency) {
    left = nullptr;
    right = nullptr;
}

huffman::Tree::Tree(Tree *left, Tree *right) : frequency(right->frequency + left->frequency), ch(-1), right(right),
                                               left(left) {}

huffman::Tree::~Tree() {
    delete right;
    delete left;
}




