#include <fstream>
#include "gtest/gtest.h"
#include "headers/bzip2.hpp"
#include "generator/random_generator.h"

static random_generator generator = random_generator();

const std::string directory = "/home/natalem/Projects/CLionProjects/CV/bzip2/google_tests/resources";

void get_random_input(std::ostream &output, size_t length) {
    for (int i = 0; i < length; i++) {
        output << (uint8_t )generator.get_int(CHAR_AMOUNT - 1);;
    }
}

bool check_same_input(std::istream &input1, std::istream &input2) {
    char ch1, ch2;
    while (input1.get(ch1)) {
        if (!input2.get(ch2))
            return false;
        if (ch2 != ch1)
            return false;
    }
    if (input2.get(ch2))
        return false;
    return true;
}


::testing::AssertionResult
test_random_data(long length, std::function<void(std::istream &input, std::ostream &output)> const &encoder,
                 std::function<void(std::istream &input, std::ostream &output)> const &decoder) {
    std::stringstream input;
    get_random_input(input, length);

    std::stringstream encoded_input;
    encoder(input, encoded_input);

    std::stringstream output;
    decoder(encoded_input, output);

    input.seekg(0, std::ios::beg);
    if (check_same_input(input, output)) {
        return ::testing::AssertionSuccess();
    } else {
        std::ofstream failed_input(directory+"/failed_input/"+std::to_string(length)+".txt");
        input.seekp(0,std::ios::beg);
        failed_input << input.rdbuf();
        return ::testing::AssertionFailure() << "random line wasn't encoded/decoded correctly";
    }
}


::testing::AssertionResult test_files(const std::string &file_name,
                                      std::function<void(std::istream &input, std::ostream &output)> const &encoder,
                                      std::function<void(std::istream &input, std::ostream &output)> const &decoder) {

    std::ifstream input(directory + "/input/" + file_name);
    std::string ss = directory + "/input/" + file_name;
    std::ofstream output(directory + "/output/" + file_name);

    encoder(input, output);
    output.close();

    std::ifstream encoded_input(directory + "/output/" + file_name);
    std::ofstream decoded_output(directory + "/decoded/" + file_name);
    decoder(encoded_input, decoded_output);
    decoded_output.close();

    std::ifstream decoded_input(directory + "/decoded/" + file_name);
    input.clear();
    input.seekg(0, std::ios::beg);

    if (check_same_input(input, decoded_input)) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure() << file_name << " file wasn't encoded/decoded correctly";
    }
}


/*____________________________________Tests_for_huffman_coding________________________________________*/

TEST(text_txt, huffman) {
    EXPECT_TRUE(test_files("Forrest_Gump_Groom_Winston.txt", huffman::encode, huffman::decode));
}

TEST(picture_jpg, huffman) {
    EXPECT_TRUE(test_files("manowar.jpg", huffman::encode, huffman::decode));
}

TEST(video_mp4, huffman) {
    EXPECT_TRUE(test_files("video.mp4", huffman::encode, huffman::decode));
}

/*____________________________________Tests_for_bzip2_coding________________________________________*/

TEST(empty_file, bzip2) {
    EXPECT_TRUE(test_files("empty.txt", bzip2::encode, bzip2::decode));
}

TEST(eng_text, bzip2) {
    EXPECT_TRUE(test_files("Animal_farm_a_fairy_story_Orwell_George.txt", bzip2::encode, bzip2::decode));
}

TEST(rus_text, bzip2) {
    EXPECT_TRUE(test_files("Leo_Tolstoy_War_and_Peace_rus.txt", bzip2::encode, bzip2::decode));
}

TEST(ital_text, bzip2) {
    EXPECT_TRUE(test_files("Leo_Tolstoy_War_and_Peace_ital.txt", bzip2::encode, bzip2::decode));
}

TEST(jap_text, bzip2) {
    EXPECT_TRUE(test_files("Osamu_Dazai_No_longer_human_jap.txt", bzip2::encode, bzip2::decode));
}

/*____________________________________Random_tests_______________________________________________________*/

TEST(random_test_10, bzip2) {
    EXPECT_TRUE(test_random_data(10, bzip2::encode, bzip2::decode));
}

TEST(random_test_100, bzip2) {
    EXPECT_TRUE(test_random_data(100, bzip2::encode, bzip2::decode));
}

TEST(random_test_1000, bzip2) {
    EXPECT_TRUE(test_random_data(1000, bzip2::encode, bzip2::decode));
}

TEST(random_test_10000, bzip2) {
    EXPECT_TRUE(test_random_data(10000, bzip2::encode, bzip2::decode));
}

TEST(random_test_100000, bzip2) {
    EXPECT_TRUE(test_random_data(100000, bzip2::encode, bzip2::decode));
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
