/**
 * TODO: file header
 *
 * Author:
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() {
    cout << "FILL" << endl;
    in.read(buf, bufSize);
    for (int i = 0; i < bufSize; i++) {
        cout << "i in buf: " << i << ": " << (unsigned int)buf[i] << endl;
    }
}

/* TODO */
bool BitInputStream::atEndOfFile() {
    int numBytesRead = in.gcount();
    if ((!in) && (numBytesRead == 0 || nbits == numBytesRead * 8)) {
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
        fill();
        return 0;
    }
    // fills buffer again
    if (nbits == bufSize * 8) {
        fill();
        nbits = 0;
    }
    int byte = nbits / 8;
    int bit = nbits % 8;

    unsigned int my_bit = ((unsigned int)buf[byte]) >> (7 - bit);
    my_bit = (1 & my_bit);
    nbits++;
    cout << "MYBIT: " << my_bit << endl;
    return my_bit;
}

BitInputStream::~BitInputStream() { delete[](buf); }
