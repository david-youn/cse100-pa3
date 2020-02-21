/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to create the BitOutputStream object and its functions. It
 * is used in conjunction with the files compress.cpp in order to properly
 * translate symbols into bits.
 */
#include "BitOutputStream.hpp"

/**
 * Writes the part of the buffer written by the user to ostream and resets
 * the buffer for further use
 * Parameter(s): none
 * Return: none
 */
void BitOutputStream::flush() {
    int bitSize = 8;
    int byte = nbits / bitSize;
    // case 1: current byte is partially filled
    if (nbits % bitSize != 0) {
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

/**
 * Writes the least significant bit of the provided integer into the buffer.
 * The method also calls flush if the buffer is full.
 * Parameter(s): i - the integer to find the least significant bit of
 * Return: none
 */
void BitOutputStream::writeBit(unsigned int i) {
    int byte = 0;
    int bit = 0;
    int bitSize = 8;
    int shiftNum = 7;
    unsigned int my_bit = 0;
    // flushes if buf is full
    if (nbits == bufSize * bitSize) {
        flush();
    }

    // calculating bit and byte position
    byte = nbits / bitSize;
    bit = (nbits % bitSize);

    // creating the bit we want to add to buf
    my_bit = (i & 1);
    my_bit = my_bit << (shiftNum - bit);

    // adding to buf at position byte at position bit
    buf[byte] = (buf[byte] | my_bit);
    nbits++;
}

/**
 * Default constructor for the BitOutputStream object
 */
BitOutputStream::~BitOutputStream() { delete[](buf); }