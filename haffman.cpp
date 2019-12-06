
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <unordered_map>

extern const int CHAR_AMOUNT;

struct huffman {
    struct Tree {
        unsigned long int frequency = 0;
        short ch = -1;
        Tree *left = nullptr;
        Tree *right = nullptr;

        Tree(unsigned long int frequency, short ch = -1) : ch(ch), frequency(frequency) {}

        Tree(Tree *left, Tree *right) : frequency(
                right->frequency + left->frequency), ch(-1), right(right), left(left) {}

        bool operator<(Tree *other) const {
            return frequency < other->frequency;
        }

        ~Tree() {
            delete right;
            delete left;
        }
    };



    static void encode(std::istream &input, std::ostream &output) {

        std::string string_(std::istreambuf_iterator<char>(input), {});
        std::vector<unsigned long int> r = count_chars(string_);
        Tree *huffmanTree = createHuffmanTree(r);
        EncodeTable table(huffmanTree);
        std::unordered_map<char, std::vector<bool>> codes = table.create_and_write_table(output);
//        output << "here";
        codeInput(string_, codes, output);
        delete huffmanTree;

        //first byte is amount of chars in data alphabet(n)
        //n bytes sequence of chars
        //uint8_t amount of bytes to code huffman tree
        //uint8_t amount of unused bits in last byte of huffman tree
        //uint32_t amount of encoded chars of data
        //uint8_t bits of encoded data


        //1 подсчитать частоту
        //2 построить дерево
        //3 построить таблицу
        //4 записать таблицу
        //5 записать закодированное сообщение
    }



    static void
    codeInput(const std::string &data, std::unordered_map<char, std::vector<bool>> &codes, std::ostream &output) {
        uint32_t data_length = data.size();
        write_uint32_t((uint32_t) data.size(), output);
//        output << (uint32_t) data.size();

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
        for (int i = cur_index + 1; i != 8; i++) {
            cur_num <<= 1;
        }
        if (cur_index != 0) output << cur_num;

    }


    static void write_uint32_t(uint32_t number, std::ostream &output) {
        uint32_t mask = 1;
        for (int i = 0; i < 7; i++) {
            mask <<= 1;
            mask |= 1;
        }
        mask <<= (3 * 8);
//        std::vector<uint8_t > num(4);
        for (int i = 3; i >= 0; i--) {

            uint32_t current_bit = mask & number;
            current_bit >>= (i * 8);
//            num[3-i] = current_bit;
            output << (uint8_t) current_bit;
            mask >>= 8;
        }

//        std::copy(num.begin(), num.end(), std::ostreambuf_iterator<char>(output));


    }

    static std::vector<unsigned long int> count_chars(const std::string &str) {
        std::vector<unsigned long int> counter(256, 0);
        long unsigned long int sum = 0;
        long unsigned long int sum2 = 0;
        for (char ch : str) {
            ++counter[ch + 127];
        }
        return counter;
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

    void createTable() {


    };


    struct bit_writer {
        std::vector<char> result;
        uint8_t current = 0;
        short index = 0;

        void add_bit(bool to_add) {
            current <<= 1;
            current |= to_add;
            if (++index == 8) {
                add_current();
            }

        }

        unsigned getSize() const {
            return result.size();
        }

        short getIndex() const {
            return index;
        }

        void flush_current() {
            if (index == 0) return;
            while (index != 8) {
                current <<= 1;
            }
            add_current();
        }

        void add_current() {
            result.push_back(current);
            index = current = 0;
        }

        void write_result(std::ostream &output) {
            flush_current();
            std::copy(result.begin(), result.end(), std::ostreambuf_iterator<char>(output));
        }

    };

    struct EncodeTable {

        EncodeTable(Tree *root) : root(root) {}

        void dfs(Tree *cur_node, std::vector<bool> &cur_code) {
            if (cur_node->left == nullptr) {//invariant: if left=nullptr then right=nullptr
                char current_char = cur_node->ch - 127;
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
            output << (uint8_t) chars.length();
            std::copy(chars.begin(), chars.end(), std::ostreambuf_iterator<char>(output));
            output << (uint8_t) writer.getSize();
            output << (uint8_t) 8 - writer.getIndex();
            writer.write_result(output);
            return codes;
        }

        Tree *root;
        std::string chars;
        bit_writer writer;
        std::unordered_map<char, std::vector<bool>> codes;
    };

};


int main() {
    std::istringstream input("aaaaabbbbsdfb");
    huffman::encode(input, std::cout);
}