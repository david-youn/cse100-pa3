/**
 * TODO: file header
 *
 * Author:
 */
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"
#include "cxxopts.hpp"

/* TODO: add pseudo compression with ascii encoding and naive header
 * (checkpoint) */
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

    for (int i = 0; i < 256; i++) {
        freqs.push_back(0);
    }

    while (true) {
        nextChar = file.get();
        if (file.eof()) {
            break;
        }
        freqs.at((int)nextChar) = freqs.at((int)nextChar) + 1;
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

    // writing the header to ofile
    for (int i = 0; i < freqs.size(); i++) {
        ofile << freqs.at(i) << endl;
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
}

/* TODO: True compression with bitwise i/o and small header (final) */
void trueCompression(string inFileName, string outFileName) {
    bool empty = true;
    HCTree tree = HCTree();
    ifstream file;
    ofstream ofile;
    BitOutputStream bos = BitOutputStream(ofile, 4000);
    unsigned char nextChar;
    file.open(inFileName);
    ofile.open(outFileName);

    // initializing a vector of ints to size 256 with value 0
    vector<unsigned int> freqs;

    for (int i = 0; i < 256; i++) {
        freqs.push_back(0);
    }

    // filling freqs vector
    while (true) {
        nextChar = file.get();
        if (file.eof()) {
            break;
        }
        freqs.at((int)nextChar) = freqs.at((int)nextChar) + 1;
    }

    int maxF = 0;
    // checking if the file is empty and also find max frequency
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            empty = false;
            // finding the maximum frequency
            if (maxF < freqs.at(i)) {
                maxF = freqs.at(i);
            }
            break;
        }
    }

    // if file is empty, return empty file
    if (empty) {
        file.close();
        ofile.close();
        return;
    }

    int freqSize = ceil(log2(maxF));

    // creating a vector to hold 1 at position for nonzero frequencies in freq
    vector<unsigned int> binaryFreqs;
    for (int i = 0; i < 256; i++) {
        binaryFreqs.push_back(0);
    }
    for (int i = 0; i < 256; i++) {
        if (freqs.at(i) != 0) {
            binaryFreqs.at(i) = 1;
        }
    }

    string strFreq = "";
    int binPos = 0;
    int bitPos = 0;
    // now translating into bits
    for (int i = 0; i < 32; i++) {
        char my_bit = 0;

        for (int j = 0; j < 8; j++) {
            if (binaryFreqs.at(binPos) != 0) {
                bitPos = binPos % 8;
                my_bit = (1 << (7 - bitPos)) | my_bit;
            }
            binPos++;
        }
        strFreq = strFreq + my_bit;
    }

    // writing the 256-bit header intro
    ofile << strFreq;

    /*
        string fSize = "";

        while (freqSize != 0) {
            if (freqSize % 2 == 1) {
                fSize = '1' + fSize;
            } else {
                fSize = '0' + fSize;
            }
            freqSize = freqSize / 2;
        }
        while (fSize.size() != 8) {
            fSize = '0' + fSize;
        }

        unsigned char byte = (unsigned char)stoi(fSize, nullptr, 2);
    */

    // writing 8-bit max frequency
    unsigned char mybyte = (char)freqSize;
    ofile << mybyte;

    // changing the frequencies into a long string of "binary"
    string binrep = "";
    for (int i = 0; i < 256; i++) {
        int freq = freqs.at(i);
        if (freq != 0) {
            for (int i = freqSize; i >= 0; i--) {
                int k = freq >> i;
                if (k & 1) {
                    binrep = binrep + "1";
                } else {
                    binrep = binrep + "0";
                }
            }
        }
    }

    ofile << "binrep: " << binrep << endl;

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
    bos.flush();
    file.close();
}

/* TODO: Main program that runs the compress */
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

    if (isAsciiOutput) {
        pseudoCompression(inFileName, outFileName);
    } else {
        trueCompression(inFileName, outFileName);
    }

    return 0;
}