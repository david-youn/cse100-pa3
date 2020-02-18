/**
 * TODO: file header
 *
 * Author:
 */
#include <fstream>
#include <iostream>

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
void trueCompression(string inFileName, string outFileName) {}

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
    }

    return 0;
}