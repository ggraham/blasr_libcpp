/*
 * =====================================================================================
 *
 *       Filename:  SMRTSequenceSequence_gtest.cpp
 *
 *    Description:  Test pbdata/SMRTSequenceSequence.h
 *
 *        Version:  1.0
 *        Created:  10/29/2012 05:17:04 PM
 *       Revision:  08/19/2014 
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include "gtest/gtest.h"
#include "SMRTSequence.hpp"

using namespace std;

Nucleotide seqnt[] = "ATATGGGGATTAGGGGATA"; 
const string seqst("ATATGGGGATTAGGGGATA"); 

SMRTSequence _make_a_smrt_read_(const string & movieName, const UInt & holeNumber,
        const DNALength subreadStart, const DNALength subreadEnd,
        const string & seqst,
        const bool hasInsertion, const bool hasDeletion, const bool hasSubstitution,
        const int insertionQVValue, const int deletionQVValue, const char deletionTagValue,
        const int substitutionQVValue, const char substitutionTagValue) {

    DNALength length = seqst.size();
    SMRTSequence smrt;
    static_cast<DNASequence*>(&smrt)->Copy(seqst);
    stringstream ss;
    ss << movieName << "/" << holeNumber << "/" << subreadStart << "_" << subreadEnd;
    smrt.CopyTitle(ss.str());

    if (hasInsertion) {
        smrt.AllocateInsertionQVSpace(length);
        smrt.insertionQV.Fill(insertionQVValue);
    }
    if (hasDeletion) {
        smrt.AllocateDeletionQVSpace(length);
        smrt.AllocateDeletionTagSpace(length);
        smrt.deletionQV.Fill(deletionQVValue);
        memset(smrt.deletionTag, deletionTagValue, sizeof(char) * length);
    }
    if (hasSubstitution) {
        smrt.AllocateSubstitutionQVSpace(length);
        smrt.AllocateSubstitutionTagSpace(length);
        smrt.substitutionQV.Fill(substitutionQVValue);
        memset(smrt.substitutionTag, substitutionTagValue, sizeof(char) * length);
    }

    smrt.SubreadStart(subreadStart);
    smrt.SubreadEnd(subreadEnd);
    smrt.HoleNumber(holeNumber);
    assert(length ==  subreadEnd - subreadStart);
    return smrt;
}

class SMRTSequenceTest: public:: testing:: Test{
public:
    void SetUp() {
        smrt.seq = seqnt;
        int len = sizeof(seqnt) / sizeof(Nucleotide) - 1;
        smrt.length = len; 
        smrt.HoleNumber(1);
        smrt.SubreadStart(0);
        smrt.SubreadEnd  (19);
        smrt.AllocateDeletionQVSpace(len);
        for(int i=0; i < 19; i ++) {
            smrt.deletionQV[i] = i;
        }
    }
    void TearDown() {
        smrt.Free();
    }
    SMRTSequence smrt;
};

TEST_F(SMRTSequenceTest, Print) {
   stringstream ss;
   smrt.Print(ss);
   ASSERT_EQ(ss.str(), (string("SMRTSequence for zmw 1, [0, 19)\n")
                        + seqst + "\n"));
}

TEST_F(SMRTSequenceTest, GetDeletionQV) {
    for(int i = 0; i < smrt.length; i ++){
        ASSERT_EQ(smrt.GetDeletionQV(i), i);
    }
}

TEST_F(SMRTSequenceTest, MadeFromSubreadsAsPolymerase) {

    string movieName = "mymovie";
    UInt holeNumber = 12354;
    string seq1 = "ATGGC";
    string seq2 = "GGCT";

    string expected_seq3 = "NATGGCNNNNGGCT";
    DNALength expected_hqstart = 0, expected_hqend = 14;
    DNALength expected_lqprefix = 1, expected_lqsuffix = 0;
    int expected_insertionQV[14]    = {0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 11, 11, 11, 11};
    int expected_deletionQV[14]     = {0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 12, 12, 12, 12};
    int expected_substitutionQV[14] = {0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 13, 13, 13, 13};
    string expected_deletionTag     = "NAAAAANNNNCCCC";
    string expected_substitutionTag = "NGGGGGNNNNTTTT";

    bool expected_hasInsertionQV = true;
    bool expected_hasDeletionQV = true;
    bool expected_hasDeletionTag = true;
    bool expected_hasSubstitutionQV = true;
    bool expected_hasSubstitutionTag = true;

    SMRTSequence read1 = _make_a_smrt_read_(movieName, holeNumber, 1, 6, seq1,
            true, true, true, 1, 2, 'A', 3, 'G');

    SMRTSequence read2 = _make_a_smrt_read_(movieName, holeNumber, 10, 14, seq2,
            true, true, true, 11, 12, 'C', 13, 'T');

    SMRTSequence read3;
    read3.MadeFromSubreadsAsPolymerase({read1, read2});

    EXPECT_EQ(read3.HoleNumber(), holeNumber);
    EXPECT_EQ(read3.SubreadStart(), expected_hqstart);
    EXPECT_EQ(read3.SubreadEnd(), expected_hqend);
    EXPECT_EQ(read3.lowQualityPrefix, expected_lqprefix);
    EXPECT_EQ(read3.lowQualitySuffix, expected_lqsuffix);
    EXPECT_EQ(read3.length, expected_seq3.size());

    EXPECT_EQ(not read3.insertionQV.Empty(), expected_hasInsertionQV);
    EXPECT_EQ(not read3.deletionQV.Empty(), expected_hasDeletionQV);
    EXPECT_EQ(not read3.substitutionQV.Empty(), expected_hasSubstitutionQV);
    EXPECT_EQ(read3.deletionTag!=nullptr, expected_hasDeletionTag);
    EXPECT_EQ(read3.substitutionTag!=nullptr, expected_hasSubstitutionTag);

    EXPECT_TRUE(read3.qual.Empty());
    EXPECT_TRUE(read3.preBaseFrames == nullptr);
    EXPECT_TRUE(read3.widthInFrames == nullptr);
    EXPECT_TRUE(read3.pulseIndex == nullptr);
    EXPECT_TRUE(read3.preBaseDeletionQV.Empty());
    EXPECT_TRUE(read3.mergeQV.Empty());

    for (size_t i = 0; i < read3.length; i++) {
        EXPECT_EQ(read3.seq[i], expected_seq3[i]);
        EXPECT_EQ(read3.insertionQV.data[i], expected_insertionQV[i]);
        EXPECT_EQ(read3.deletionQV.data[i], expected_deletionQV[i]);
        EXPECT_EQ(read3.deletionTag[i], expected_deletionTag[i]);
        EXPECT_EQ(read3.substitutionQV.data[i], expected_substitutionQV[i]);
        EXPECT_EQ(read3.substitutionTag[i], expected_substitutionTag[i]);
    }
}


TEST_F(SMRTSequenceTest, MadeFromSubreadsAsPolymeraseNoInsertionNoDeletion) {

    string movieName = "mymovie";
    UInt holeNumber = 12354;
    string seq1 = "ATGGC";
    string seq2 = "GGCT";

    string expected_seq3 = "NATGGCNNNNGGCT";
    DNALength expected_hqstart = 0, expected_hqend = 14;
    DNALength expected_lqprefix = 1, expected_lqsuffix = 0;
    int expected_substitutionQV[14] = {0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 13, 13, 13, 13};
    string expected_substitutionTag = "NGGGGGNNNNTTTT";

    bool expected_hasInsertionQV = false;
    bool expected_hasDeletionQV = false;
    bool expected_hasDeletionTag = false;
    bool expected_hasSubstitutionQV = true;
    bool expected_hasSubstitutionTag = true;

    SMRTSequence read1 = _make_a_smrt_read_(movieName, holeNumber, 1, 6, seq1,
            false, true, true, 1, 2, 'A', 3, 'G');
    SMRTSequence read2 = _make_a_smrt_read_(movieName, holeNumber, 10, 14, seq2,
            true, true, true, 11, 12, 'C', 13, 'T');

    // read3 should contain neither insertionQV nor deletionQV nor deletionTag
    SMRTSequence read3;
    read3.MadeFromSubreadsAsPolymerase({read1, read2});

    EXPECT_EQ(read3.HoleNumber(), holeNumber);
    EXPECT_EQ(read3.SubreadStart(), expected_hqstart);
    EXPECT_EQ(read3.SubreadEnd(), expected_hqend);
    EXPECT_EQ(read3.lowQualityPrefix, expected_lqprefix);
    EXPECT_EQ(read3.lowQualitySuffix, expected_lqsuffix);
    EXPECT_EQ(read3.length, expected_seq3.size());

    EXPECT_EQ(not read3.insertionQV.Empty(), expected_hasInsertionQV);
    EXPECT_EQ(not read3.deletionQV.Empty(), expected_hasDeletionQV);
    EXPECT_EQ(not read3.substitutionQV.Empty(), expected_hasSubstitutionQV);
    EXPECT_EQ(read3.deletionTag!=nullptr, expected_hasDeletionTag);
    EXPECT_EQ(read3.substitutionTag!=nullptr, expected_hasSubstitutionTag);

    EXPECT_TRUE(read3.qual.Empty());
    EXPECT_TRUE(read3.preBaseFrames == nullptr);
    EXPECT_TRUE(read3.widthInFrames == nullptr);
    EXPECT_TRUE(read3.pulseIndex == nullptr);
    EXPECT_TRUE(read3.preBaseDeletionQV.Empty());
    EXPECT_TRUE(read3.mergeQV.Empty());

    for (size_t i = 0; i < read3.length; i++) {
        EXPECT_EQ(read3.seq[i], expected_seq3[i]);
        EXPECT_EQ(read3.substitutionQV.data[i], expected_substitutionQV[i]);
        EXPECT_EQ(read3.substitutionTag[i], expected_substitutionTag[i]);
    }
}

TEST_F(SMRTSequenceTest, MadeFromSubreadsAsPolymeraseNoInsertionNoDeletionNoSubstitution) {

    string movieName = "mymovie";
    UInt holeNumber = 12354;
    string seq1 = "ATGGC";
    string seq2 = "GGCT";

    string expected_seq3 = "NATGGCNNNNGGCT";
    DNALength expected_hqstart = 0, expected_hqend = 14;
    DNALength expected_lqprefix = 1, expected_lqsuffix = 0;

    bool expected_hasInsertionQV = false;
    bool expected_hasDeletionQV = false;
    bool expected_hasDeletionTag = false;
    bool expected_hasSubstitutionQV = false;
    bool expected_hasSubstitutionTag = false;

    SMRTSequence read1 = _make_a_smrt_read_(movieName, holeNumber, 1, 6, seq1,
            false, true, true, 1, 2, 'A', 3, 'G');
    SMRTSequence read2 = _make_a_smrt_read_(movieName, holeNumber, 10, 14, seq2,
            true, true, false, 11, 12, 'C', 13, 'T');

    // read3 should contain neither insertionQV nor deletionQV nor deletionTag
    SMRTSequence read3;
    read3.MadeFromSubreadsAsPolymerase({read1, read2});

    EXPECT_EQ(read3.HoleNumber(), holeNumber);
    EXPECT_EQ(read3.SubreadStart(), expected_hqstart);
    EXPECT_EQ(read3.SubreadEnd(), expected_hqend);
    EXPECT_EQ(read3.lowQualityPrefix, expected_lqprefix);
    EXPECT_EQ(read3.lowQualitySuffix, expected_lqsuffix);
    EXPECT_EQ(read3.length, expected_seq3.size());

    EXPECT_EQ(not read3.insertionQV.Empty(), expected_hasInsertionQV);
    EXPECT_EQ(not read3.deletionQV.Empty(), expected_hasDeletionQV);
    EXPECT_EQ(not read3.substitutionQV.Empty(), expected_hasSubstitutionQV);
    EXPECT_EQ(read3.deletionTag!=nullptr, expected_hasDeletionTag);
    EXPECT_EQ(read3.substitutionTag!=nullptr, expected_hasSubstitutionTag);

    EXPECT_TRUE(read3.qual.Empty());
    EXPECT_TRUE(read3.preBaseFrames == nullptr);
    EXPECT_TRUE(read3.widthInFrames == nullptr);
    EXPECT_TRUE(read3.pulseIndex == nullptr);
    EXPECT_TRUE(read3.preBaseDeletionQV.Empty());
    EXPECT_TRUE(read3.mergeQV.Empty());

    for (size_t i = 0; i < read3.length; i++) {
        EXPECT_EQ(read3.seq[i], expected_seq3[i]);
    }
}
