/*
 * =====================================================================================
 *
 *       Filename:  NucConversion_gtest.cpp
 *
 *    Description:  Test pbdata/NucConversion.hpp
 *
 *        Version:  1.0
 *        Created:  08/19/2014 05:21:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include "NucConversion.hpp"
#include "gtest/gtest.h"

// ACGT = 0123
TEST(NucConversion, ASCIITo2BIT)
{
    EXPECT_EQ(TwoBit[static_cast<size_t>('A')], 0);
    EXPECT_EQ(TwoBit[static_cast<size_t>('a')], 0);
    EXPECT_EQ(TwoBit[static_cast<size_t>('C')], 1);
    EXPECT_EQ(TwoBit[static_cast<size_t>('c')], 1);
    EXPECT_EQ(TwoBit[static_cast<size_t>('G')], 2);
    EXPECT_EQ(TwoBit[static_cast<size_t>('g')], 2);
    EXPECT_EQ(TwoBit[static_cast<size_t>('T')], 3);
    EXPECT_EQ(TwoBit[static_cast<size_t>('t')], 3);
    EXPECT_EQ(TwoBit[static_cast<size_t>('N')], 255);
    EXPECT_EQ(TwoBit[static_cast<size_t>('x')], 255);
}

TEST(NucConversion, ASCIITo3BIT)
{
    EXPECT_EQ(ThreeBit[static_cast<size_t>('A')], 0);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('a')], 0);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('C')], 1);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('c')], 1);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('G')], 2);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('g')], 2);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('T')], 3);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('t')], 3);

    EXPECT_EQ(ThreeBit[static_cast<size_t>('U')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('M')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('R')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('W')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('S')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('Y')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('K')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('V')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('H')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('D')], 4);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('N')], 4);

    EXPECT_EQ(ThreeBit[static_cast<size_t>('$')], 5);

    EXPECT_EQ(ThreeBit[static_cast<size_t>('p')], 255);
    EXPECT_EQ(ThreeBit[static_cast<size_t>('q')], 255);
}

TEST(NucConversion, ISACTG)
{
    EXPECT_TRUE(IsACTG[static_cast<size_t>('A')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('a')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('C')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('c')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('G')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('g')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('T')]);
    EXPECT_TRUE(IsACTG[static_cast<size_t>('t')]);

    EXPECT_FALSE(IsACTG[static_cast<size_t>('w')]);
    EXPECT_FALSE(IsACTG[static_cast<size_t>('N')]);
}

TEST(NucConversion, ThreeBitToASCII)
{
    char alphabeta[] = {'A', 'C', 'G', 'T'};
    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(alphabeta[i], ThreeBitToAscii[ThreeBit[static_cast<size_t>(alphabeta[i])]]);
    }
}

TEST(NucConversion, AllToUpper)
{
    char alphabeta[] = {'A', 'C', 'G', 'T', 'a', 'c', 'g', 't'};
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(toupper(alphabeta[i]), AllToUpper[static_cast<size_t>(alphabeta[i])]);
    }
}

TEST(NucConversion, AllToLower)
{
    char alphabeta[] = {'A', 'C', 'G', 'T', 'a', 'c', 'g', 't'};
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(tolower(alphabeta[i]), AllToLower[static_cast<size_t>(alphabeta[i])]);
    }
}

TEST(NucConversion, ReverseComplementNuc)
{
    EXPECT_EQ(ReverseComplementNuc[static_cast<size_t>('A')], 'T');
    EXPECT_EQ(ReverseComplementNuc[static_cast<size_t>('T')], 'A');
    EXPECT_EQ(ReverseComplementNuc[static_cast<size_t>('G')], 'C');
    EXPECT_EQ(ReverseComplementNuc[static_cast<size_t>('C')], 'G');
}
