#include <iostream>
#include <functional>
#include <fstream>
#include "move_to_front.hpp"
#include "bwt.hpp"

bool check_same_input(std::istream &input1, std::istream &input2) {
    char ch1, ch2;
    while (input1.get(ch1)) {
        if (!input2.get(ch2))return false;
        if (ch2 != ch1) return false;
    }
    if (input2.get(ch2)) return false;
    return true;
}

void create_file_with_input(std::ostream &output, std::string_view input) {
    output << input;
}

bool check_coding_decoding(std::string_view input_string, std::string test_name_file,
                           std::function<void(std::istream &input, std::ostream &output)> const &coder,
                           std::function<void(std::istream &input, std::ostream &output)> const &decoder) {

    std::ofstream file_input("input_" + test_name_file + ".txt");
    create_file_with_input(file_input, input_string);
    file_input.close();

    std::ifstream input("input_" + test_name_file + ".txt");
    std::ofstream coded_output("coded_" + test_name_file + ".txt");
    coder(input, coded_output);
    input.clear();
    input.seekg(0, std::ios::beg);
    coded_output.close();

    std::ifstream coded_input("coded_" + test_name_file + ".txt");
    std::ofstream decoded_output("decoded_" + test_name_file + ".txt");
    decoder(coded_input, decoded_output);
    decoded_output.close();
    coded_input.close();

    std::ifstream decoded_input("decoded_" + test_name_file + ".txt");
    return check_same_input(input, decoded_input);
}

void test_move_to_front(std::string_view str_test) {
    if (check_coding_decoding(str_test, "MTF", move_to_front::encode, move_to_front::decode)) std::cout << "passed MTF\n";
    else std::cout << "not passed MTF\n";
}

void test_BWT(std::string_view str_test) {
    if (check_coding_decoding(str_test, "BWT", BWT::transform, BWT::inverseTransform)) std::cout << "passed BWT\n";
    else std::cout << "not passed BWT\n";
}

void code_decode_jpag(){
    std::ifstream jpg("manowar.jpg");
    std::ofstream jpg_out("manowar_mtf_encoded.jpg");

    move_to_front::encode(jpg, jpg_out);
    jpg_out.close();
    jpg.close();

    jpg = std::ifstream("manowar_mtf_encoded.jpg");
    jpg_out = std::ofstream("manowar_mtf_decoded.jpg");
    move_to_front::decode(jpg, jpg_out);

    jpg_out.close();
    jpg.close();
    jpg = std::ifstream("manowar.jpg");
    std::ifstream jpg2("manowar_mtf_decoded.jpg");
   if ( check_same_input(jpg, jpg2)) std::cout << "passed";

}

void code_decode_jpag_2(){
    std::ifstream jpg("manowar.jpg");
    std::ofstream jpg_out("manowar_mtf_encoded.jpg");

    BWT::transform(jpg, jpg_out);
    jpg_out.close();
    jpg.close();

    jpg = std::ifstream("manowar_mtf_encoded.jpg");
    jpg_out = std::ofstream("manowar_mtf_decoded.jpg");
    BWT::inverseTransform(jpg, jpg_out);

    jpg_out.close();
    jpg.close();
    jpg = std::ifstream("manowar.jpg");
    std::ifstream jpg2("manowar_mtf_decoded.jpg");
    if ( check_same_input(jpg, jpg2)) std::cout << "passed";

}




int main1() {

    test_move_to_front("a\nbdflaskj;lkjgd;выаыодфлавыо\n\n\n\nвыалдд!!!23453");
    test_BWT("fskljdlgfdldjf;d\n\nlgjkldsflk\nlkfsdflakj;sldkj;-gfj;lkjfds;lkj;lk\n\n\nkfsdjljgl\n");

//   code_decode_jpag_2();

    return 0;
}
