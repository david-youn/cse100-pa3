/**
 * Names: David Youn - A15452585
 *        Jonathan Yun - A15431969
 *
 * Sources: Piazza
 *
 * This file is used to define the HCTree object as well as its available
 * functions. It is used in conjunction with files corresponding to
 * BitInputStream, BitOutputStream, and HCNode.
 */
#include "HCTree.hpp"

priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

/**
 * Helper recursive function to the default destructor that iterates through
 * the entire tree and deletes every node.
 */
static void deleteHelper(HCNode* n) {
    // base case that returns if there are no more nodes
    if (n == nullptr) {
        return;
    }
    // recursively goes through the left sub-tree of the tree
    if (n->c0 != nullptr) {
        deleteHelper(n->c0);
    }
    // recursively goes through the right sub-tree of the tree
    if (n->c1 != nullptr) {
        deleteHelper(n->c1);
    }
    // deletes the node and sets pointer to null
    delete (n);
    n = nullptr;
}

/**
 * Defualt destructor for the HCTree object
 */
HCTree::~HCTree() {
    while (pq.size() != 0) {
        delete pq.top();
        pq.pop();
    }
    deleteHelper(root);
    root = nullptr;
}

/**
 * Method that creates the HCTree by using the provided frequency vector
 * and creates a node for each char with a nonzero frequency.
 * Parameter(s): freqs - a vector of size 256 that contains the frequency
 *                       of the char corresponding to the vector index
 * Return: none
 */
void HCTree::build(const vector<unsigned int>& freqs) {
    int asciiSize = 256;
    // creating a forest of single-node trees in the PQ
    for (int i = 0; i < asciiSize; i++) {
        // creating nodes for each of the non-zero values
        if (freqs.at(i) != 0) {
            HCNode* n = new HCNode(freqs.at(i), i, nullptr, nullptr, nullptr);

            // adding to leaves vector
            leaves.push_back(n);

            // adding to pq
            pq.push(n);
        } else {
            // nullptr for symbols that dont appear
            leaves.push_back(nullptr);
        }
    }
    // no nodes in tree
    if (pq.size() == 0) {
        return;
    }
    while (pq.size() != 1) {
        HCNode* n1 = pq.top();
        pq.pop();
        HCNode* n2 = pq.top();
        pq.pop();

        // creating parent node that takes c0's symbol
        HCNode* p =
            new HCNode(n1->count + n2->count, n1->symbol, n1, n2, nullptr);
        // merge trees
        n1->p = p;
        n2->p = p;

        // pushing the parent back into the queue
        pq.push(p);
    }
    root = pq.top();
    pq.pop();
}

/**
 * Method that translates a provided byte into its encoded bits into
 * the provided BitOutputStream.
 * Parameter(s): symbol - the symbol to encode
 *              out - the BitOutputStream to write to
 * Return: none
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
    HCNode* curr = nullptr;
    HCNode* prev = nullptr;
    string code = "";

    curr = leaves.at(symbol);

    // for the case of just having one node in the HCT
    if (curr == root) {
        out.writeBit(0);
    }

    while (curr != root) {
        prev = curr;
        curr = curr->p;

        // checking if c0 or c1
        if (curr->c0 == prev) {
            code = '0' + code;
        } else {
            code = '1' + code;
        }
    }

    for (int i = 0; i < code.length(); i++) {
        out.writeBit(code.at(i));
    }
}

/**
 * Method that translates a provided byte into its ascii binary
 * representation into the provided ostream.
 * Parameter(s): symbol - the symbol to encode
 *              out - the ostream to write to
 * Return: none
 */
void HCTree::encode(byte symbol, ostream& out) const {
    HCNode* curr = nullptr;
    HCNode* prev = nullptr;
    string code = "";

    curr = leaves.at(symbol);

    // for the case of just having one node in the HCT
    if (curr == root) {
        out << "0";
    }

    while (curr != root) {
        prev = curr;
        curr = curr->p;

        // checking if c0 or c1
        if (curr->c0 == prev) {
            code = '0' + code;
        } else {
            code = '1' + code;
        }
    }
    out << code;
}

/**
 * Method that decodes a sequence of bits into its corresponding symbol
 * Parameter(s): in - the BitInputStream to read the sequence of bits from
 * Return: the byte of the decoded symbol
 */
byte HCTree::decode(BitInputStream& in) const {
    HCNode* curr = root;
    unsigned int c;
    // iterates through entire tree and finds the binary representation
    while (curr->c0 != nullptr && curr->c1 != nullptr) {
        c = in.readBit();
        if (c == 0) {
            curr = curr->c0;
        } else {
            curr = curr->c1;
        }
    }
    return curr->symbol;
}

/**
 * Method that decodes a sequence of ascii values into its corresponding
 * symbol
 * Parameter(s): in - the istream to read the sequence of values from
 * Return: the byte of the decoded symbol
 */
byte HCTree::decode(istream& in) const {
    HCNode* curr = root;
    char c;
    // iterates through entire tree and finds the binary representation
    while (curr->c0 != nullptr && curr->c1 != nullptr) {
        in.get(c);
        if (c == '0') {
            curr = curr->c0;
        } else {
            curr = curr->c1;
        }
    }
    return curr->symbol;
}