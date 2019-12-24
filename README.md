# BZIP2 compression program

Own realization of bzip2 using BWT, move to front and huffman algorithms.

Modular structure permits to use any of three coding algorithms separately. 

Efficiently works with text files.

Allows to compress all types of data(text, video, pictures).

## Performance

**__1.5 to 4 times text data compression__**

Examples:

* Leo Tolstoy _"War and Peace"_ in russian 

    _(1240 Kb vs 320 Kb)_

* George Orwell _"Animal farm: a fairy story"_ 

    _(160 Kb vs 54 Kb)_
    
For more examples see 

```bzip2/google_tests/resources```

## GoogleTest

### Setup

To run tests:

1) Create folder "lib" in google_tests : 
    ```bzip2/google_tests/lib``` 
2) Download googleTest source files 
    (https://github.com/google/googletest) 
    and decompress in created folder google_tests/lib
    
### Information
* Tests huffman encoding/decoding for different types of data

* Tests bzip2 text compression for different languages

* Tests with random data for bzip2 realization

_Tests can be easily modified to check work of modules for bzip2_

## Binary representation of huffman encoding/decoding

* first 4 bytes decode amount (__n__) of chars in data alphabet
* __n__ bytes of chars
* 1 byte for amount (__m__) of bytes to encode huffman tree
* __m__ bytes of compressed bit huffman tree (for more information check [itmo.wiki][wiki])
* 4 bytes for amount of chars in input data
* encoded data

## Meta

Telegram: @natallem

Email: lnv2000@inbox.ru

[https://github.com/natallem](https://github.com/natallem)

[wiki]: https://neerc.ifmo.ru/wiki/index.php?title=%D0%9E%D0%BF%D1%82%D0%B8%D0%BC%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D0%B5_%D1%85%D1%80%D0%B0%D0%BD%D0%B5%D0%BD%D0%B8%D0%B5_%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D1%80%D1%8F_%D0%B2_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%B5_%D0%A5%D0%B0%D1%84%D1%84%D0%BC%D0%B0%D0%BD%D0%B0
