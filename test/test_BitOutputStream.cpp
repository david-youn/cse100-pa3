#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include "BitOutputStream.hpp"

using namespace std;
using namespace testing;

TEST(BitOutputStreamTests, SIMPLE_TEST) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(1);
    bos.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, SIMPLE_TEST2) {
    stringstream ss;
    BitOutputStream bos(ss, 10);
    bos.writeBit(1);
    bos.flush();

    string bitsStr = "10000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, SIMPLE_TEST3) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(1);
    bos.writeBit(0);
    bos.writeBit(1);
    bos.flush();

    string bitsStr = "10100000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, SIMPLE_TEST4) {
    stringstream ss;
    BitOutputStream bos(ss, 3);
    bos.writeBit(1);
    bos.writeBit(0);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);

    bos.flush();

    string bitsStr = "10111111";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
    string bitsStr2 = "11111111";
    asciiVal = stoi(bitsStr2, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
    string bitsStr3 = "10000000";
    asciiVal = stoi(bitsStr3, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, SIMPLE_TEST5) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(1);
    bos.writeBit(0);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);
    bos.writeBit(1);

    bos.flush();

    string bitsStr = "10111111";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}

TEST(BitOutputStreamTests, SIMPLE_TEST6) {
    stringstream ss;
    BitOutputStream bos(ss, 1);
    bos.writeBit(0);
    bos.flush();

    string bitsStr = "00000000";
    unsigned int asciiVal = stoi(bitsStr, nullptr, 2);
    ASSERT_EQ(ss.get(), asciiVal);
}