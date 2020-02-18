/**
 * TODO: file header
 *
 * Author:
 */
#include "BitOutputStream.hpp"

/* TODO */
void BitOutputStream::flush() {
    cout << "nbits = " << nbits << endl;
    int byte = nbits / 8;
    cout << "byte: " << byte << endl;
    // case 1: current byte is partially filled
    if (nbits % 8 != 0) {
        for (int i = 0; i < byte + 1; i++) {
            cout << " in for loop buf[i]: " << buf[i] << endl;
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
    cout << "nbits: " << nbits << endl;
    // flushes if buf is full
    if (nbits == bufSize * 8) {
        flush();
    }

    // calculating bit and byte position
    byte = nbits / 8;
    bit = (nbits % 8);

    // creating the bit we want to add to buf
    my_bit = (i & 1);
    cout << "my_bit before shift: " << my_bit << endl;
    my_bit = my_bit << (7 - bit);
    cout << "my_bit after shift: " << my_bit << endl;

    // adding to buf at position byte at position bit
    cout << "buf[byte] before: " << buf[byte] << endl;
    buf[byte] = (buf[byte] | my_bit);
    cout << "buf[byte] after: " << buf[byte] << endl;

    cout << "nbits hither: " << nbits << endl;
    nbits++;
    cout << "nbits after ++: " << nbits << endl;
}
