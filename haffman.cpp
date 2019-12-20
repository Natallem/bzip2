
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <fstream>

extern const int CHAR_AMOUNT;

struct huffman {

    struct Tree {
        unsigned long int frequency = 0;
        short ch = -1;
        Tree *left = nullptr;
        Tree *right = nullptr;

        Tree(unsigned long int frequency, short ch = -1) : ch(ch), frequency(frequency) {
            left = nullptr;
            right = nullptr;
        }

        Tree(Tree *left, Tree *right) : frequency(right->frequency + left->frequency), ch(-1), right(right),
                                        left(left) {}

        ~Tree() {
            delete right;
            delete left;
        }
    };

    struct bit_writer {
        void add_bit(bool bit) {
            byte <<= 1;
            byte |= bit;
            if (++shift == 8) {
                add_byte();
            }
        }

        unsigned getSize() const {
            return result.size();
        }

        short getIndex() const {
            if (shift == 0) return 8;
            return shift;
        }

        void flush_byte() {
            if (shift == 0) return;
            while (shift++ < 8) {
                byte <<= 1;
            }
            add_byte();
        }

        void add_byte() {
            result.push_back(byte);
            shift = byte = 0;
        }

        void write_result(std::ostream &output) {
            std::copy(result.begin(), result.end(), std::ostreambuf_iterator<char>(output));
        }

    private:
        std::vector<uint8_t> result;
        uint8_t byte = 0;
        short shift = 0;

    };

    static void write_uint32_t(uint32_t number, std::ostream &output) {
        uint32_t mask = 1;
        for (int i = 0; i < 7; i++) {
            mask <<= 1;
            mask |= 1;
        }
        mask <<= (3 * 8);
        for (int i = 3; i >= 0; i--) {
            uint32_t current_bit = mask & number;
            current_bit >>= (i * 8);
            output << (uint8_t) current_bit;
            mask >>= 8;
        }
    }

    static void encode(std::istream &input, std::ostream &output) {

//        std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
//                                         std::istreambuf_iterator<char>());
//
        std::string string_(std::istreambuf_iterator<char>(input), {});
        std::vector<unsigned long int> r(256, 0);
        count_chars(string_, r);
        Tree *huffmanTree = createHuffmanTree(r);
        EncodeTable table(huffmanTree);
        std::unordered_map<char, std::vector<bool>> codes = table.create_and_write_table(output);
        table.encodeData(string_, output);
        delete huffmanTree;
//        first byte is amount of chars in data alphabet(n)
//        n bytes sequence of chars
//        uint8_t amount of bytes to code huffman tree
//        uint8_t amount of unused bits in last byte of huffman tree
//        uint32_t amount of encoded chars of data
//        uint8_t bits of encoded data


        //1 подсчитать частоту
        //2 построить дерево
        //3 построить таблицу
        //4 записать таблицу
        //5 записать закодированное сообщение
    }

    static void count_chars(const std::string &input_chars, std::vector<unsigned long int> &counter) {
        for (char ch : input_chars) {
            ++counter.at(ch + 128);
        }
    }

    static Tree *createHuffmanTree(const std::vector<unsigned long int> &counter) {

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

    struct EncodeTable {

        explicit EncodeTable(Tree *root) : root(root) {}

        void dfs(Tree *cur_node, std::vector<bool> &cur_code) {
            if (cur_node->left == nullptr) {//invariant: if left=nullptr then right=nullptr
                char current_char = cur_node->ch - 128;
                chars += current_char;
                codes.insert({current_char, cur_code});
                return;
            }

            writer.add_bit(0);
            cur_code.push_back(0);
            dfs(cur_node->left, cur_code);
            cur_code.pop_back();

            cur_code.push_back(1);
            writer.add_bit(1);
            dfs(cur_node->right, cur_code);
            cur_code.pop_back();
        }

        std::unordered_map<char, std::vector<bool>> create_and_write_table(std::ostream &output) {
            std::vector<bool> cur_code;
            dfs(root, cur_code);
//            writer.add_bit(1);
            output << (uint8_t) chars.length();
            std::copy(chars.begin(), chars.end(), std::ostreambuf_iterator<char>(output));

            uint8_t a = (uint8_t) 8 - writer.getIndex();
            writer.flush_byte();
            uint8_t b = writer.getSize();
            output << (uint8_t) writer.getSize();

            output << a;
            writer.write_result(output);
            return codes;
        }

        void encodeData(const std::string &data, std::ostream &output) {
            uint32_t data_length = data.size();
            write_uint32_t((uint32_t) data.size(), output);
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
            }
            output << cur_num;
        }

        Tree *root;
        std::string chars;
        bit_writer writer;
        std::unordered_map<char, std::vector<bool>> codes;
    };

    static void decode(std::istream &input, std::ostream &output) {
        DecodeTable table(input);//create Huffman decode tree
        table.decode(input, output);
    }

    struct DecodeTable {

        //first byte is amount of chars in data alphabet(n)
        //n bytes sequence of chars
        //uint8_t amount of bytes to code huffman tree
        //uint8_t amount of unused bits in last byte of huffman tree
        //uint32_t amount of encoded chars of data
        //uint8_t bits of encoded data

        DecodeTable(std::istream &input) {
            createHuffmanTree(input);
        }

        uint32_t read_uint32_t(std::istream &input) {
            std::vector<uint8_t> vec(4);
            uint8_t t;
            uint8_t t2t;
            input >> t;
            uint32_t tt = 0;
            std::copy_n(std::istreambuf_iterator<char>{input}, 5, begin(vec));
            for (int i = 3; i >= 0; i--) {
                uint32_t k = vec[3 - i] << (8 * i);
                tt |= (vec[3 - i] << (8 * i));
            }

            return tt;
        }

        void decode(std::istream &input, std::ostream &output) {
            uint32_t encoded_chars_amount;
            encoded_chars_amount = read_uint32_t(input);
            std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(input)),
                                             std::istreambuf_iterator<char>());
            Tree *cur_node = root;
            uint32_t decoded_chars_counter = 0;
            std::string s;
            for (auto byte : input_chars) {
/*
                for (int i = (1 << 7); i > 0; i = (i >> 1)) {
                    uint8_t k = byte & i;
                    if (k==0) s+='0';
                            else s+='1';
                }
*/
                for (int i = (1 << 7); i > 0; i >>= 1) {
                    if (i & byte) {
                        cur_node = cur_node->right;
                    } else {
                        cur_node = cur_node->left;
                    }
                    if (cur_node->left == nullptr) {
                        char ch = cur_node->ch;
//                        std::cout << "cout char \n";
                        output << ch;
                        if (++decoded_chars_counter == encoded_chars_amount) {
//                            std::cout  << '\0';
                            return;
                        };
                        cur_node = root;
                    }
                }
            }

        }

        void createHuffmanTree(std::istream &input) {
            uint8_t chars_amount;
            input >> chars_amount;
            alphabet.resize(chars_amount);
            std::copy_n(std::istreambuf_iterator<char>{input}, chars_amount + 1, begin(alphabet));
            uint8_t amount_bytes_huffman;
            uint8_t unused_bits_huffman;
            input >> amount_bytes_huffman;
            input >> unused_bits_huffman;
            bits_of_encoded_huffman = 8 * amount_bytes_huffman - unused_bits_huffman;
            bytes.resize(amount_bytes_huffman);
            std::copy_n(std::istreambuf_iterator<char>{input}, amount_bytes_huffman, begin(bytes));
            root = dfs();
        }

        Tree *dfs() {
            bool b = (ptr_alphabet == alphabet.size() - 1) ? true : read_bit(ptr_huffman++);
            if (b) {
                return new Tree(0, alphabet[ptr_alphabet++]);
            } else {
                Tree *node = new Tree(0, -1);
                node->left = dfs();
                node->right = dfs();
                return node;
            }
        };

        bool read_bit(int index) {
            int ptr_byte = (index) / 8;
            int ptr_bit = (index) % 8;
            uint8_t a = (1 << (7 - ptr_bit));
            return (bytes[ptr_byte] & a);
        }

        std::vector<uint8_t> bytes;
        int ptr_huffman = 0;
        int ptr_alphabet = 0;

        Tree *root;
        std::vector<uint8_t> alphabet;
        uint32_t bits_of_encoded_huffman;

    };

};

void readAllBits(std::istream &istream) {
    std::vector<uint8_t> input_chars((std::istreambuf_iterator<char>(istream)),
                                     std::istreambuf_iterator<char>());
    for (auto ch : input_chars) {
        for (int i = (1 << 7); i > 0; i = (i >> 1)) {
            uint8_t k = ch & i;
            std::cout << (bool) (k != 0);
//            std::cout << "(" <<(int) ch << ")";
        }
        std::cout << "(" << (int) ch << ")";
        std::cout << "\n";
    }
}


int main() {
//    std::istringstream input("aaaaabbbbsdfb");
    std::ifstream input("Leo_Tolstoy_War_and_Peace_rus.in");
    std::ofstream fileout("output.txt");
    huffman::encode(input, fileout);
    fileout.close();
    std::ifstream filein("output.txt");
    std::ofstream fileout2("Leo_Tolstoy_War_and_Peace_rus.out");
    readAllBits(filein);
    filein.clear();
    filein.seekg(0, std::ios::beg);
    huffman::decode(filein, fileout2);
//    std::string s;


}