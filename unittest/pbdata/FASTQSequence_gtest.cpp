/*
 * =====================================================================================
 *
 *       Filename:  FASTQSequence_gtest.cpp
 *
 *    Description:  Test pbdata/FASTQSequence.hpp
 *
 *        Version:  1.0
 *        Created:  1/13/2017 05:19:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include <climits>
#include <fstream>
#include <iostream>
#include "FASTQSequence.hpp"
#include "gtest/gtest.h"

class FASTQSequenceTest : public ::testing::Test
{
public:
    virtual void SetUp() {}

    virtual void TearDown() { fastqOne.Free(); }

    FASTQSequence fastqOne;

    std::streambuf* sbuf;
    std::ofstream ofs;
};

TEST_F(FASTQSequenceTest, ReverseComplementSelf)
{
    EXPECT_TRUE(fastqOne.title == NULL);
    EXPECT_TRUE(fastqOne.titleLength == 0);
    EXPECT_TRUE(fastqOne.seq == NULL);
    EXPECT_TRUE(fastqOne.length == 0);
    EXPECT_FALSE(fastqOne.deleteOnExit);
    EXPECT_TRUE(fastqOne.qual.Empty());
    EXPECT_TRUE(fastqOne.deletionQV.Empty());
    EXPECT_TRUE(fastqOne.insertionQV.Empty());
    EXPECT_TRUE(fastqOne.substitutionQV.Empty());
    EXPECT_TRUE(fastqOne.mergeQV.Empty());
    EXPECT_TRUE(fastqOne.preBaseDeletionQV.Empty());
    EXPECT_EQ(fastqOne.GetStorageSize(), 0);

    const std::string name = "fastq_seq_one comments";
    const std::string s = "TTAGCTAG";
    const std::string q = "23!abcde";
    const std::string r = "CTAGCTAA";
    const std::string rq = "edcba!32";
    fastqOne.CopyTitle(name);

    EXPECT_EQ(fastqOne.title, name);
    static_cast<FASTASequence*>(&fastqOne)->Copy(s);
    EXPECT_EQ(fastqOne.length, s.size());

    fastqOne.qual.Copy(q);
    EXPECT_EQ(fastqOne.qual.ToString(), q);

    // Test ReverseComplementSelf()
    fastqOne.ReverseComplementSelf();
    EXPECT_EQ(memcmp(fastqOne.seq, r.c_str(), r.size() * sizeof(char)), 0);
    EXPECT_EQ(fastqOne.title, name);
    EXPECT_EQ(fastqOne.qual.ToString(), rq);
}
