/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to declare method headers and variables used in
 * BitOutputStream.cpp. It is used in conjunction with the compress.cpp file
 * to properly translate symbols into encoded bits.
 */
#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/**
 * This class is used to define the method headers used for methods
 * corresponding to the BitOutputStream object. It contains the variables out,
 * buf, bufSize, and nbits, which are used to create a buffer as well as
 * read and fill the buffer.
 */
class BitOutputStream {
  private:
    ostream& out;          // reference to the output stream to use
    char* buf;             // buffer of an char array
    unsigned int bufSize;  // size of buffer in byte
    unsigned int nbits;    // number of bits have been writen to buf

  public:
    /**
     * Constructor that creates a BitOutputStream object with the given ostream
     * and bufSize, creating a the buffer.
     * Parameter(s): os - the ostream to set the variable "out" to
     *               bufSize - the size the buffer "buf" should be
     * Return: none
     */
    explicit BitOutputStream(ostream& os, unsigned int bufSize) : out(os) {
        // initializing member variables
        this->bufSize = bufSize;
        buf = new char[bufSize];
        for (int i = 0; i < bufSize; i++) {
            buf[i] = NULL;
        }
        nbits = 0;
    };

    /**
     * Writes the part of the buffer written by the user to ostream and resets
     * the buffer for further use
     * Parameter(s): none
     * Return: none
     */
    void flush();

    /**
     * Writes the least significant bit of the provided integer into the buffer.
     * The method also calls flush if the buffer is full.
     * Parameter(s): i - the integer to find the least significant bit of
     * Return: none
     */
    void writeBit(unsigned int i);

    /**
     * Default constructor for the BitOutputStream object
     */
    ~BitOutputStream();
};

#endif