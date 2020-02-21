/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to declare method headers and variables used in
 * HCTree.cpp. It is used in conjunction with files corresponding to
 * BitInputStream, BitOutputStream, and HCNode.
 */
#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <fstream>
#include <queue>
#include <vector>
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"
#include "HCNode.hpp"

using namespace std;

/**
 * This class is used to define the method headers used for methods
 * corresponding to the HCTree object. It contains the variables root and
 * leaves, which are used to construct the tree during implementation.
 */
class HCTree {
  private:
    HCNode* root;            // the root of HCTree
    vector<HCNode*> leaves;  // a vector storing pointers to all leaf HCNodes

  public:
    /**
     * Default constructor that sets the root pointer to null
     * Parameter(s): none
     * Return: none
     */
    HCTree() { root = nullptr; }

    /**
     * Defualt destructor for the HCTree object
     */
    ~HCTree();

    /**
     * Method that creates the HCTree by using the provided frequency vector
     * and creates a node for each char with a nonzero frequency.
     * Parameter(s): freqs - a vector of size 256 that contains the frequency
     *                       of the char corresponding to the vector index
     * Return: none
     */
    void build(const vector<unsigned int>& freqs);

    /**
     * Method that translates a provided byte into its encoded bits into
     * the provided BitOutputStream.
     * Parameter(s): symbol - the symbol to encode
     *              out - the BitOutputStream to write to
     * Return: none
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /**
     * Method that translates a provided byte into its ascii binary
     * representation into the provided ostream.
     * Parameter(s): symbol - the symbol to encode
     *              out - the ostream to write to
     * Return: none
     */
    void encode(byte symbol, ostream& out) const;

    /**
     * Method that decodes a sequence of bits into its corresponding symbol
     * Parameter(s): in - the BitInputStream to read the sequence of bits from
     * Return: the byte of the decoded symbol
     */
    byte decode(BitInputStream& in) const;

    /**
     * Method that decodes a sequence of ascii values into its corresponding
     * symbol
     * Parameter(s): in - the istream to read the sequence of values from
     * Return: the byte of the decoded symbol
     */
    byte decode(istream& in) const;
};

#endif  // HCTREE_HPP
