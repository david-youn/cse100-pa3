/**
 * TODO: file header
 *
 * Author:
 */
#include "BitOutputStream.hpp"

/* TODO */
void BitOutputStream::flush() {
    int byte = nbits / 8;
    // case 1: current byte is partially filled
    if (nbits % 8 != 0) {
        for (int i = 0; i < byte + 1; i++) {
            out << buf[i];
        }
    }
    // case 2: current byte is entirely filled
    else {
        for (int i = 0; i < byte; i++) {
            out << buf[i];
        }
    }

    // zero-filling buf
    for (int i = 0; i < bufSize; i++) {
        buf[i] = NULL;
    }
    nbits = 0;
}

/* TODO */
void BitOutputStream::writeBit(unsigned int i) {
    int byte = 0;
    int bit = 0;
    unsigned int my_bit = 0;
    // flushes if buf is full
    if (nbits == bufSize * 8) {
        flush();
    }

    // calculating bit and byte position
    byte = nbits / 8;
    bit = (nbits % 8);

    // creating the bit we want to add to buf
    my_bit = (i & 1);
    my_bit = my_bit << (7 - bit);

    // adding to buf at position byte at position bit
    buf[byte] = (buf[byte] | my_bit);
    nbits++;
}

BitOutputStream::~BitOutputStream() { delete[](buf); }
