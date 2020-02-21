/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to declare method headers and variables used in
 * BitInputStream.cpp. It is used in conjunction with the uncompress.cpp file
 * to properly translate encoded bits into symbols.
 */
#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/**
 * This class is used to define the method headers used for methods
 * corresponding to the BitInputStream object. It contains the variables in,
 * buf, bufSize, eofBit and nbits, which are used to create a buffer as well as
 * read and fill the buffer.
 */
class BitInputStream {
  private:
    istream& in;           // reference to the input stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits that have been read from buf
    bool eofBit;           // true when trying to read past the end of the file

    /**
     * Method that returns whether or not there are no more bits remaining
     * Parameter(s): none
     * Return: boolean representing whether or not istream attempts to read
     * more bytes than the amount left in the file, as well as if the buffer
     * was fully read.
     */
    bool atEndOfFile();

  public:
    /**
     * Constructor that creates a BitInputStream object using istream "is"
     * and bufSize and creates the buffer
     * Parameter(s): is - the istream to read the bytes from
     *               bufSize - the size of the buffer
     * Return: none
     */
    explicit BitInputStream(istream& is, unsigned int bufSize) : in(is) {
        // initializing member variables
        this->bufSize = bufSize;
        buf = new char[bufSize];
        for (int i = 0; i < bufSize; i++) {
            buf[i] = NULL;
        }
        nbits = 0;
        eofBit = false;
    };

    /**
     * Fills the buffer using the istream
     * Parameter(s): none
     * Return: none
     */
    void fill();

    /**
     * Returns eofBit, telling whether the program is trying to read past
     * the end of the file.
     * Parameter(s): none
     * Return: eofBit variable
     */
    bool eof();

    /**
     * Reads the next bit within the buffer and returns the bit
     * Parameter(s): none
     * Return: unsigned int of either 1 or 0, depending on what was read
     */
    unsigned int readBit();

    /**
     * Default destructor
     */
    ~BitInputStream();
};

#endif