/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {
    cout << "FILL" << endl;
    unsigned char nextChar;
    for (int i = 0; i < bufSize; i++) {
        nextChar = in.get();
        cout << "nextChar: " << (unsigned int)nextChar << endl;
        buf[i] = nextChar;
    }
}

/* TODO */
bool BitInputStream::atEndOfFile() {
    int numBytesRead = in.gcount();
    cout << "(!(in)): " << (!in) << endl;
    cout << "in.gcount() == 0: " << (numBytesRead == 0) << endl;
    cout << "nbits: " << nbits << endl;
    cout << "in.gcount() == nbits * 8 : " << (numBytesRead == nbits * 8)
         << endl;
    if ((!in) && (numBytesRead == 0 || numBytesRead == nbits * 8) &&
        (nbits == bufSize * 8)) {
        cout << "eof hit here" << endl;
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
    cout << "NOT aEOF" << endl;
    // fills buffer again
    if (nbits == bufSize * 8) {
        fill();
    }
    int byte = nbits / 8;
    int bit = nbits % 8;

    unsigned int my_bit = ((unsigned int)buf[byte]) >> (7 - bit);
    my_bit = (1 & my_bit);
    nbits++;
    return my_bit;
}

BitInputStream::~BitInputStream() { delete[](buf); }
