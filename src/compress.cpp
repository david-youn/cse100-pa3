/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to regulate user interaction and works for the compression
 * of files. It is used in conjunction with other files including
 * BitOutputStream.cpp and HCTree.
 */
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/**
 * Method that translates every char in a given file into its ascii values
 * Parameter(s): infFIleName - the name of the file to translate
 *               outFileName - the name of the file to insert the translation
 * Return: none
 */
void pseudoCompression(string inFileName, string outFileName) {
    bool empty = true;
    HCTree tree = HCTree();
    ifstream file;
    ofstream ofile;
    unsigned char nextChar;
    file.open(inFileName);
    ofile.open(outFileName);

    // initializing a vector of ints to size 256 with value 0
    vector<unsigned int> freqs;
    int asciiSize = 256;

    for (int i = 0; i < asciiSize; i++) {
        freqs.push_back(0);
    }

    while (true) {
        nextChar = file.get();
        if (file.eof()) {
            break;
        }
        freqs.at((int)nextChar) = freqs.at((int)nextChar) + 1;
    }

    // writing the header to ofile
    for (int i = 0; i < freqs.size(); i++) {
        ofile << freqs.at(i) << endl;
    }
    // checking if the file is empty
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            empty = false;
            break;
        }
    }

    // if file is empty, return empty file
    if (empty) {
        file.close();
        ofile.close();
        return;
    }

    tree.build(freqs);
    file.close();
    file.open(inFileName);

    // going through inFileName and creating the frequency vector
    while (true) {
        nextChar = file.get();
        if (file.eof()) {
            break;
        }
        tree.encode(nextChar, ofile);
    }
    file.close();
    ofile.close();
}

/**
 * Method that compresses a given file and also writes the header to assist
 * decompression
 * Parameter(s): inFileName - the file to compress
 *               outFileName - the file to insert the compression
 * Return: none
 */
void trueCompression(string inFileName, string outFileName) {
    bool empty = true;
    HCTree tree = HCTree();
    ifstream file;
    ofstream ofile;
    // creates buffer of default sie of 4000
    BitOutputStream bos = BitOutputStream(ofile, 4000);
    unsigned char nextChar;
    file.open(inFileName);
    ofile.open(outFileName);

    // initializing a vector of ints to size 256 with value 0
    vector<unsigned int> freqs;
    int asciiSize = 256;

    // fills frequency vector with 0s
    for (int i = 0; i < asciiSize; i++) {
        freqs.push_back(0);
    }

    // filling freqs vector
    while (true) {
        nextChar = file.get();
        if (file.eof()) {
            break;
        }
        // inserts freqeuncy of corresponding ascii index
        freqs.at((int)nextChar) = freqs.at((int)nextChar) + 1;
    }

    // checks if the file is empty or if there are no frequencies
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            empty = false;
            break;
        }
    }

    // if file is empty, return empty file
    if (empty) {
        file.close();
        ofile.close();
        return;
    }

    unsigned int maxF = 0;
    // checking if the file is empty and also find max frequency
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            // finding the maximum frequency
            if (maxF < freqs.at(i)) {
                maxF = freqs.at(i);
            }
        }
    }

    // holds the maximum number of bits requred to hold the frequencies of the
    // the symbols
    int freqSize = ceil(log2(maxF));

    // creating a vector to hold 1 at position for nonzero frequencies in freq
    vector<unsigned int> binaryFreqs;
    for (int i = 0; i < asciiSize; i++) {
        binaryFreqs.push_back(0);
    }
    // finding corresponding indices where frequencies are nonzero
    for (int i = 0; i < asciiSize; i++) {
        if (freqs.at(i) != 0) {
            binaryFreqs.at(i) = 1;
        }
    }

    string strFreq = "";
    int binPos = 0;
    int bitPos = 0;
    // variables used to get rid of magic numbers, describe different sizes
    int byteSize = 32;
    int bitSize = 8;
    int shiftSize = 7;
    // now translating into bits
    for (int i = 0; i < byteSize; i++) {
        char my_bit = 0;
        // iterates through every bit in each byte
        for (int j = 0; j < bitSize; j++) {
            // checks if there exists a frequency at corresponding bit
            if (binaryFreqs.at(binPos) != 0) {
                bitPos = binPos % bitSize;
                // insertes bit into byte
                my_bit = (1 << (shiftSize - bitPos)) | my_bit;
            }
            // continues to iterate through binaryFreqs
            binPos++;
        }
        strFreq = strFreq + my_bit;
    }

    // writing the 256-bit header intro
    ofile << strFreq;

    // writing 8-bit max frequency
    unsigned char mybyte = (char)freqSize;
    ofile << mybyte;

    // changing the frequencies into a long string of "binary"
    string binrep = "";
    for (int i = 0; i < asciiSize; i++) {
        int freq = freqs.at(i);
        if (freq != 0) {
            // translates the frequency into a string of binary
            for (int i = freqSize - 1; i >= 0; i--) {
                int k = freq >> i;
                if (k & 1) {
                    binrep = binrep + "1";
                } else {
                    binrep = binrep + "0";
                }
            }
        }
    }

    // until binrep is divisible by 8, append 0's to end
    while ((binrep.size()) % bitSize != 0) {
        binrep = binrep + "0";
    }

    // going through and converting the binary string to actual bits
    for (int i = 0; i < binrep.size(); i = i + bitSize) {
        unsigned char byte = (char)stoi(binrep.substr(i, bitSize), nullptr, 2);
        ofile << byte;
    }

    tree.build(freqs);
    file.close();
    file.open(inFileName);

    // going through inFileName and creating the frequency vector
    while (true) {
        nextChar = file.get();
        if (file.eof()) {
            break;
        }
        tree.encode(nextChar, bos);
    }
    // clears the buffer
    bos.flush();
    file.close();
    ofile.close();
}

/**
 * Main method that regulates user interaction and input and calls the
 * corresponding function based on flages by user
 * Parameter(s): argc - default argument counting number of passed arguments
 *               argv[] - default array parameter that hold user inputs
 * Return: int - default return type for main method
 */
int main(int argc, char* argv[]) {
    cxxopts::Options options("./compress",
                             "Compresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Write output in ascii mode instead of bit stream",
        cxxopts::value<bool>(isAsciiOutput))(
        "input", "", cxxopts::value<string>(inFileName))(
        "output", "", cxxopts::value<string>(outFileName))(
        "h,help", "Print help and exit");

    options.parse_positional({"input", "output"});
    auto userOptions = options.parse(argc, argv);

    if (userOptions.count("help") || !FileUtils::isValidFile(inFileName) ||
        outFileName.empty()) {
        cout << options.help({""}) << std::endl;
        exit(0);
    }
    // checks for user flags
    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}