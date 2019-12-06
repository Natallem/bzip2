#ifndef BWT_HPP
#define BWT_HPP

extern const int CHAR_AMOUNT;

struct BWT
{
    // Преобразование Барроуза-Уилера, этап кодирования
    static void transform(std::istream &input, std::ostream &output);

    // Обратное преобразование Барроуза-Уилера, этап декодирования
    static void inverseTransform(std::istream &input, std::ostream &output);

};

#endif //BWT_HPP
