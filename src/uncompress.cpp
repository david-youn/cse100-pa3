/**
 * TODO: file header
 *
 * Author:
 */
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>

#include <cxxopts.hpp>
#include "FileUtils.hpp"
#include "HCNode.hpp"
#include "HCTree.hpp"

/* TODO: Pseudo decompression with ascii encoding and naive header (checkpoint)
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

    for (int i = 0; i < 256; i++) {
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

/* TODO: True decompression with bitwise i/o and small header (final) */
void trueDecompression(string inFileName, string outFileName) {
    HCTree tree = HCTree();
    ifstream file;
    ofstream ofile;
    BitInputStream BIS = BitInputStream(file, 4000);

    unsigned char nextChar;
    file.open(inFileName);
    ofile.open(outFileName);

    vector<unsigned int> freqs;

    for (int i = 0; i < 256; i++) {
        freqs.push_back(0);
    }

    string brep = "";

    // reading the first 32 bytes from header
    for (int i = 0; i < 32; i++) {
        char c = file.get();
        int asc = (int)c;

        for (int i = 7; i >= 0; i--) {
            int k = asc >> i;
            if (k & 1) {
                brep = brep + "1";
            } else {
                brep = brep + "0";
            }
        }
    }

    int nonZeroNum = 0;
    // now converting brep into array with 1's and 0's for frequency
    for (int i = 0; i < 256; i++) {
        if (brep.at(i) == '1') {
            freqs.at(i) = 1;
            nonZeroNum++;
        }
    }

    // now converting the 8-bit max frequency value
    char f = file.get();
    int f_asc = (int)f;

    int numBits = nonZeroNum * f_asc;
    if (numBits % 8 != 0) {
        numBits = numBits + (8 - (numBits % 8));
    }
    int numBytes = numBits / 8;

    // reading in the next numBytes worth of data
    string nrep = "";
    for (int i = 0; i < numBytes; i++) {
        char next = file.get();
        int n_asc = (int)next;

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
        just_freqs.push_back(actualFreq);
    }

    int counter = 0;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs.at(i) != 0) {
            freqs.at(i) = just_freqs.at(counter);
            counter++;
        }
    }

    unsigned int charNum = 0;
    for (int i = 0; i < 256; i++) {
        charNum = charNum + freqs.at(i);
    }

    tree.build(freqs);
    for (int i = 0; i < charNum; i++) {
        ofile << tree.decode(BIS);
    }
    file.close();
    ofile.close();
}

/* TODO: Main program that runs the uncompress */
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

    if (isAsciiOutput) {
        pseudoDecompression(inFileName, outFileName);
    } else {
        trueDecompression(inFileName, outFileName);
    }
    return 0;
}
