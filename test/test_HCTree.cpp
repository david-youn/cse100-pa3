#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "HCTree.hpp"

using namespace std;
using namespace testing;

class SimpleHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree;

  public:
    SimpleHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs['a'] = 2;
        freqs['b'] = 3;
        freqs['c'] = 4;
        freqs['d'] = 5;
        freqs['e'] = 3;
        tree.build(freqs);
    }
};

TEST_F(SimpleHCTreeFixture, TEST_ENCODE) {
    ostringstream os;
    tree.encode('a', os);
    ASSERT_EQ(os.str(), "110");
}

TEST_F(SimpleHCTreeFixture, TEST_REAL_ENCODE) {
    stringstream os;
    BitOutputStream bs = BitOutputStream(os, 10);
    tree.encode('a', bs);
    bs.flush();
    ASSERT_EQ(os.str(), "\xC0");
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE1) {
    ostringstream os;
    tree.encode('b', os);
    ASSERT_EQ(os.str(), "00");
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE2) {
    ostringstream os;
    tree.encode('c', os);
    ASSERT_EQ(os.str(), "01");
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE3) {
    ostringstream os;
    tree.encode('d', os);
    ASSERT_EQ(os.str(), "10");
}

TEST_F(SimpleHCTreeFixture, TEST_ENCODE4) {
    ostringstream os;
    tree.encode('e', os);
    ASSERT_EQ(os.str(), "111");
}

TEST_F(SimpleHCTreeFixture, TEST_DECODE) {
    istringstream is("00");
    ASSERT_EQ(tree.decode(is), 'b');
}

TEST_F(SimpleHCTreeFixture, TEST_REAL_DECODE) {
    istringstream is("\x40");
    BitInputStream in = BitInputStream(is, 10);
    ASSERT_EQ(tree.decode(in), 'c');
}

class AdvancedHCTreeFixture : public ::testing::Test {
  protected:
    HCTree tree2;

  public:
    AdvancedHCTreeFixture() {
        // initialization code here
        vector<unsigned int> freqs(256);
        freqs[' '] = 27;
        freqs['+'] = 32;
        freqs['D'] = 15;
        freqs['a'] = 12;
        freqs['J'] = 15;
        freqs['o'] = 7;
        freqs['Z'] = 10;
        freqs['n'] = 7;
        freqs['t'] = 1800;
        freqs['h'] = 300;
        tree2.build(freqs);
    }
};

TEST_F(AdvancedHCTreeFixture, TEST_ENCODEA) {
    ostringstream os;
    tree2.encode(' ', os);
    ASSERT_EQ(os.str(), "0000");
}

TEST_F(AdvancedHCTreeFixture, TEST_ENCODEB) {
    ostringstream os;
    tree2.encode('+', os);
    ASSERT_EQ(os.str(), "0010");
}

TEST_F(AdvancedHCTreeFixture, TEST_ENCODEC) {
    ostringstream os;
    tree2.encode('o', os);
    ASSERT_EQ(os.str(), "000100");
}

TEST_F(AdvancedHCTreeFixture, TEST_ENCODED) {
    ostringstream os;
    tree2.encode('Z', os);
    ASSERT_EQ(os.str(), "001110");
}

TEST_F(AdvancedHCTreeFixture, TEST_ENCODEE) {
    ostringstream os;
    tree2.encode('h', os);
    ASSERT_EQ(os.str(), "01");
}

TEST_F(AdvancedHCTreeFixture, TEST_DECODEA) {
    istringstream is("1");
    ASSERT_EQ(tree2.decode(is), 't');
}

TEST_F(AdvancedHCTreeFixture, TEST_DECODEB) {
    istringstream is("000101");
    ASSERT_EQ(tree2.decode(is), 'n');
}

TEST_F(AdvancedHCTreeFixture, TEST_DECODEC) {
    istringstream is("00110");
    ASSERT_EQ(tree2.decode(is), 'D');
}

TEST_F(AdvancedHCTreeFixture, TEST_REAL_DECODE1) {
    istringstream is("\x30");
    BitInputStream in = BitInputStream(is, 11);
    ASSERT_EQ(tree2.decode(in), 'D');
}