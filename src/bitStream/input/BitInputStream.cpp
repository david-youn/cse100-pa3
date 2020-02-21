/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to create the BitInputStream object and its functions. It
 * is used in conjunction with the files uncompress.cpp in order to properly
 * translate bits into symbols.
 */
#include "BitInputStream.hpp"

/* TODO */
void BitInputStream::fill() { in.read(buf, bufSize); }

/**
 * Fills the buffer using the istream
 * Parameter(s): none
 * Return: none
 */
bool BitInputStream::atEndOfFile() {
    int bitSize = 8;
    int numBytesRead = in.gcount();
    if ((!in) && (numBytesRead == 0 || nbits == numBytesRead * bitSize)) {
        return true;
    }
    return false;
}

/* Returns true when trying to read past the end of the file, false otherwise */
bool BitInputStream::eof() { return eofBit; }

/**
 * Reads the next bit within the buffer and returns the bit
 * Parameter(s): none
 * Return: unsigned int of either 1 or 0, depending on what was read
 */
unsigned int BitInputStream::readBit() {
    int bitSize = 8;
    int shiftNum = 7;
    // fills buffer again
    if (nbits % (bufSize * bitSize) == 0) {
        fill();
        nbits = 0;
    }

    // case if we reach the end of the file
    if (atEndOfFile()) {
        eofBit = true;
        fill();
        return 0;
    }

    int byte = nbits / bitSize;
    int bit = nbits % bitSize;

    // finding the least significant bit
    unsigned int my_bit = ((unsigned int)buf[byte]) >> (shiftNum - bit);
    my_bit = (1 & my_bit);
    nbits++;
    return my_bit;
}

/**
 * Default destructor
 */
BitInputStream::~BitInputStream() { delete[](buf); }