/**
 * TODO: file header
 *
 * Author:
 */
#include "HCTree.hpp"

priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

static void deleteHelper(HCNode* n) {
    if (n == nullptr) {
        return;
    }
    // recursively goes through the left sub-tree of the tree
    deleteHelper(n->c0);
    // recursively goes through the right sub-tree of the tree
    deleteHelper(n->c1);
    delete (n);
    n = nullptr;
}

/* TODO */
HCTree::~HCTree() {
    while (pq.size() != 0) {
        delete pq.top();
        pq.pop();
    }
    deleteHelper(root);
    root = nullptr;
}

/* TODO */
void HCTree::build(const vector<unsigned int>& freqs) {
    // creating a forest of single-node trees in the PQ
    for (int i = 0; i < 256; i++) {
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

/* TODO */
// void HCTree::encode(byte symbol, BitOutputStream& out) const {}

/* TODO */
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

/* TODO */
// byte HCTree::decode(BitInputStream& in) const { return ' '; }

/* TODO */
byte HCTree::decode(istream& in) const {
    HCNode* curr = root;
    char c;
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
