/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {
    unsigned char nextChar;
    cout << "fill" << endl;
    for (int i = 0; i < bufSize; i++) {
        nextChar = in.get();
        cout << "nextChar: " << (unsigned int)nextChar << endl;
        buf[i] = nextChar;
        cout << "buf[i]: " << (unsigned int)buf[i] << endl;
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
    if (nbits == 0) {
        fill();
    }
    int byte = nbits / 8;
    int bit = nbits % 8;
    cout << "bit: " << bit << endl;
    cout << "byte: " << byte << endl;

    for (int i = 0; i < bufSize; i++) {
        cout << "i: " << (unsigned int)buf[i] << endl;
    }
    cout << "buf[byte]: " << buf[byte] << endl;
    unsigned int my_bit = ((unsigned int)buf[byte]) >> (7 - bit);
    my_bit = (1 & my_bit);
    cout << "my_bit: " << my_bit << endl;
    nbits++;
    return my_bit;
}