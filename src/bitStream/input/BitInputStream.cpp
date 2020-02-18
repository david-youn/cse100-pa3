/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {
    unsigned char nextChar;
    for (int i = 0; i < bufSize; i++) {
        nextChar = in.get();
        buf[i] = nextChar;
    }
}

/* TODO */
bool BitInputStream::atEndOfFile() {
    if ((!in) && (in.gcount() == 0 || in.gcount() == nbits * 8)) {
        return true;
    }
    return false;
}

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/* TODO */
unsigned int BitInputStream::readBit() {
    if (atEndOfFile()) {
        eofBit = true;
        return 0;
    }

    int byte = nbits / 8;
    int bit = nbits % 8;
    unsigned int my_bit = 1 << (7 - nbits);
    my_bit = (buf[byte] | my_bit);
    nbits++;
    return my_bit >> (7 - nbits);
}