/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to regulate user interaction and works for the
 * uncompression of files. It is used in conjunction with other files
 * including BitInputStream.cpp and HCTree.
 */
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

#include <cxxopts.hpp>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/**
 * Method that translates ascii values in a given file into its char
 * representation
 * Parameter(s): infFIleName - the name of the file to translate
 *               outFileName - the name of the file to insert the translation
 * Return: none
 */
void pseudoDecompression(string inFileName, string outFileName) {
    HCTree tree = HCTree();
    ifstream file;
    ofstream ofile;
    unsigned char nextChar;
    file.open(inFileName);
    ofile.open(outFileName);
    vector<unsigned int> freqs;

    unsigned int charNum = 0;
    int asciiSize = 256;

    // reads the header
    for (int i = 0; i < asciiSize; i++) {
        string line;
        getline(file, line);
        unsigned int f = atoi(line.c_str());
        charNum = charNum + f;
        freqs.push_back(f);
    }

    tree.build(freqs);
    for (int i = 0; i < charNum; i++) {
        ofile << tree.decode(file);
    }
    file.close();
    ofile.close();
}

/**
 * Method that decompresses a given file by using decompressed header and
 * creating resulting HCTree.
 * Parameter(s): inFileName - the file to decompress
 *               outFileName - the file to insert the original text
 * Return: none
 */
void trueDecompression(string inFileName, string outFileName) {
    HCTree tree = HCTree();
    ifstream file;
    ofstream ofile;
    BitInputStream BIS = BitInputStream(file, 4000);

    unsigned char nextChar;
    file.open(inFileName);
    ofile.open(outFileName);

    vector<unsigned int> freqs;
    // variable that holds size of ascii characters
    int asciiSize = 256;
    // holds number of chars in header
    int charSize = 32;
    // used to fill bit in char
    int shiftSize = 7;
    int bitSize = 8;

    for (int i = 0; i < asciiSize; i++) {
        freqs.push_back(0);
    }

    string brep = "";

    // reading the first 32 bytes from header
    for (int i = 0; i < charSize; i++) {
        char c = file.get();
        int asc = (int)c;
        // translates every bit into a string of binary
        for (int i = shiftSize; i >= 0; i--) {
            int k = asc >> i;
            if (k & 1) {
                brep = brep + "1";
            } else {
                brep = brep + "0";
            }
        }
    }

    int nonZeroNum = 0;
    // now converting the binary string into array with 1's and 0's for
    // frequency
    for (int i = 0; i < asciiSize; i++) {
        if (brep.at(i) == '1') {
            freqs.at(i) = 1;
            nonZeroNum++;
        }
    }

    // now converting the 8-bit max frequency value
    char f = file.get();
    int f_asc = (int)f;

    // finds the number of remaining bytes in the header after finding first 2
    // sections
    int numBits = nonZeroNum * f_asc;
    if (numBits % bitSize != 0) {
        numBits = numBits + (bitSize - (numBits % bitSize));
    }
    int numBytes = numBits / bitSize;

    // reading in the next numBytes worth of data
    string nrep = "";
    for (int i = 0; i < numBytes; i++) {
        char next = file.get();
        int n_asc = (int)next;
        // writes out remaining bytes of file into a string representing the
        // binary values
        for (int i = 7; i >= 0; i--) {
            int k = n_asc >> i;
            if (k & 1) {
                nrep = nrep + "1";
            } else {
                nrep = nrep + "0";
            }
        }
    }

    vector<unsigned int> just_freqs;
    for (int i = 0; i < nonZeroNum; i++) {
        // finds the binary representation of the frequencies of the chars
        // in the remaining section of the header
        string fr = nrep.substr(i * f_asc, f_asc);
        unsigned int actualFreq = 0;
        int power = fr.size() - 1;
        // manually converting the string representation binary to its int
        for (int k = 0; k < fr.size(); k++) {
            if (fr.at(k) == '1') {
                actualFreq = actualFreq + pow(2, power);
            }
            power--;
        }
        // inserts the decoded frequencies into the frequency holder vector
        just_freqs.push_back(actualFreq);
    }

    // fills the actual frequency vector with the proper frequencies
    int counter = 0;
    for (int i = 0; i < freqs.size(); i++) {
        // inserts the actual frequency in the proper index
        if (freqs.at(i) != 0) {
            freqs.at(i) = just_freqs.at(counter);
            counter++;
        }
    }

    // ifnds the number of total characters within the entire file
    unsigned int charNum = 0;
    for (int i = 0; i < asciiSize; i++) {
        charNum = charNum + freqs.at(i);
    }

    // creates HCTree and decodes the bits
    tree.build(freqs);
    for (int i = 0; i < charNum; i++) {
        ofile << tree.decode(BIS);
    }
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
    cxxopts::Options options("./uncompress",
                             "Uncompresses files using Huffman Encoding");
    options.positional_help("./path_to_input_file ./path_to_output_file");

    bool isAsciiOutput = false;
    string inFileName, outFileName;
    options.allow_unrecognised_options().add_options()(
        "ascii", "Read output in ascii mode instead of bit stream",
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

    // calls corresponding functions depending of the user's flag
    if (isAsciiOutput) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }
    return 0;
}