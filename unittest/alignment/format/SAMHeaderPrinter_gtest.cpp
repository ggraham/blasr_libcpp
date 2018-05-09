/*
 * =====================================================================================
 *
 *       Filename:  SAMHeaderPrinter_gtest.cpp
 *
 *    Description:  Test alignment/format/SAMHeaderPrinter.hpp
 *
 *        Version:  1.0
 *        Created:  03/24/2015 04:51:29 PM
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include <algorithm>
#include <cstdio>
#include <cstring>

#include <gtest/gtest.h>

#define private public
#define protected public

#include "FASTAReader.hpp"
#include "format/SAMHeaderPrinter.hpp"
#include "pbdata/testdata.h"

class SAMHeaderPrinterTest : public testing::Test
{
public:
    void SetUp()
    {
        samQVs.SetDefaultQV();
        so = "UNKNOWN";
        readType = ReadType::SUBREAD;

        int rand;
        std::string fastaFn(fastaFile1);
        reader.computeMD5 = true;
        EXPECT_TRUE(reader.Init(fastaFn));
        reader.ReadAllSequencesIntoOne(sequence, &seqdb);
    }

    void TearDown()
    {
        if (printer) {
            delete printer;
            printer = NULL;
        }
    }

    SAMHeaderPrinter* printer;
    SupplementalQVList samQVs;
    std::string so;
    ReadType::ReadTypeEnum readType;
    FASTAReader reader;
    FASTASequence sequence;
    SequenceIndexDatabase<FASTASequence> seqdb;
};

const std::string bax1ExpectedHeader =
    "PU:m130220_114643_42129_c100471902550000001823071906131347_s1_p0\tPL:PACBIO\tDS:READTYPE="
    "SUBREAD;BINDINGKIT=;SEQUENCINGKIT=;BASECALLERVERSION=2.0;InsertionQV=iq;DeletionQV=dq;"
    "SubstitutionQV=sq;MergeQV=mq;DeletionTag=dt";

const std::string bax3ExpectedHeader =
    "PU:m150223_190837_42175_c100735112550000001823160806051530_s1_p0\tPL:PACBIO\tDS:READTYPE="
    "SUBREAD;BINDINGKIT=100356300;SEQUENCINGKIT=100356200;BASECALLERVERSION=2.3;InsertionQV=iq;"
    "DeletionQV=dq;SubstitutionQV=sq;MergeQV=mq;DeletionTag=dt";

const std::string pls1ExpectedHeader =
    "PU:m121215_065521_richard_c100425710150000001823055001121371_s1_p0\tPL:PACBIO\tDS:READTYPE="
    "SUBREAD;BINDINGKIT=;SEQUENCINGKIT=;BASECALLERVERSION=1.3;InsertionQV=iq;DeletionQV=dq;"
    "SubstitutionQV=sq;MergeQV=mq;DeletionTag=dt";

TEST_F(SAMHeaderPrinterTest, BAX_ONE_MOVIE_IN)
{
    // Read from two bax files of the same movie.
    EXPECT_EQ(readType, ReadType::ReadTypeEnum::SUBREAD);
    std::vector<std::string> readsFiles = {baxFile1, baxFile2};
    printer = new SAMHeaderPrinter(so, seqdb, readsFiles, readType, samQVs, "blasr", "1.3.2",
                                   "blasr a b c");

    EXPECT_EQ(printer->_hd.ToString().find("@HD\tVN:1.5\tSO:UNKNOWN\tpb:3.0.1"), 0u);
    EXPECT_EQ(printer->_sqs._groups.size(), 12u);
    EXPECT_EQ(printer->_sqs._groups[0].ToString().find("@SQ\tSN:read1\tLN:100\tM5:"), 0u);
    EXPECT_EQ(printer->_sqs._groups[11].ToString().find("@SQ\tSN:read2x\tLN:100\tM5:"), 0u);

    // Expect exactly one read group
    EXPECT_EQ(printer->_rgs._groups.size(), 1u);
    EXPECT_NE(printer->_rgs._groups[0].ToString().find(bax1ExpectedHeader), std::string::npos);

    EXPECT_EQ(printer->_pgs._groups.size(), 1u);
    EXPECT_EQ(printer->_cos._groups.size(), 0u);
}

TEST_F(SAMHeaderPrinterTest, BAX_MULTI_MOVIE_IN)
{
    // Read subread from more than one movies
    std::vector<std::string> readsFiles = {baxFile1, baxFile2, baxFile3, plsFile1};
    printer = new SAMHeaderPrinter(so, seqdb, readsFiles, readType, samQVs, "blasr", "1.3.2",
                                   "blasr a b c");

    EXPECT_EQ(printer->_hd.ToString().find("@HD\tVN:1.5\tSO:UNKNOWN\tpb:3.0.1"), 0u);
    EXPECT_EQ(printer->_sqs._groups.size(), 12u);
    EXPECT_EQ(printer->_sqs._groups[0].ToString().find("@SQ\tSN:read1\tLN:100\tM5:"), 0u);
    EXPECT_EQ(printer->_sqs._groups[11].ToString().find("@SQ\tSN:read2x\tLN:100\tM5:"), 0u);

    // Expect three read groups because baxFile1 and baxFile2 contains reads of the same movie.
    EXPECT_EQ(printer->_rgs._groups.size(), 3u);

    EXPECT_NE(printer->_rgs._groups[0].ToString().find(bax1ExpectedHeader), std::string::npos);
    EXPECT_NE(printer->_rgs._groups[1].ToString().find(bax3ExpectedHeader), std::string::npos);
    EXPECT_NE(printer->_rgs._groups[2].ToString().find(pls1ExpectedHeader), std::string::npos);

    EXPECT_EQ(printer->_pgs._groups.size(), 1u);
    EXPECT_EQ(printer->_cos._groups.size(), 0u);
}

const std::string bam1ExpectedHeader =
    "@RG\tID:b89a4406\tPL:PACBIO\tDS:READTYPE=SUBREAD;DeletionQV=dq;DeletionTag=dt;InsertionQV=iq;"
    "MergeQV=mq;SubstitutionQV=sq;Ipd:CodecV1=ip;BINDINGKIT=100356300;SEQUENCINGKIT=100356200;"
    "BASECALLERVERSION=2.3.0.0.140018;FRAMERATEHZ=75.000000\t"
    "PU:m140905_042212_sidney_c100564852550000001823085912221377_s1_X0\tPM:SEQUEL";
const std::string bam2ExpectedHeader =
    "PL:PACBIO\tDS:READTYPE=SUBREAD;DeletionQV=dq;DeletionTag=dt;InsertionQV=iq;MergeQV=mq;"
    "SubstitutionQV=sq;Ipd=ip;BINDINGKIT=100236500;SEQUENCINGKIT=001558034;BASECALLERVERSION=2.3.0."
    "1.142990\tPU:m150325_224749_42269_c100795290850000001823159309091522_s1_p0";

TEST_F(SAMHeaderPrinterTest, ONE_BAM_IN)
{
    // Read the same file twice in order to test uniqueness of @RG
    std::vector<std::string> readsFiles = {bamFile1, bamFile1};
    printer = new SAMHeaderPrinter(so, seqdb, readsFiles, readType, samQVs, "blasr", "1.3.2",
                                   "blasr a b c");

    EXPECT_EQ(printer->_rgs._groups.size(), 1u);
    EXPECT_EQ(printer->_rgs._groups[0].ToString(), bam1ExpectedHeader);

    EXPECT_EQ(printer->_pgs._groups.size(), 3u);
}

// TEST_F(SAMHeaderPrinterTest, TWO_BAM_IN)
// {
//     // Read multiple bam files
//     std::vector<std::string> readsFiles = {bamFile1, bamFile2};
//     printer = new SAMHeaderPrinter(so, seqdb, readsFiles, readType, samQVs, "blasr", "1.3.2",
//                                    "blasr a b c");

//     EXPECT_EQ(printer->_rgs._groups.size(), 2);
//     EXPECT_NE(printer->_rgs._groups[0].ToString().find(bam1ExpectedHeader), std::string::npos);
//     EXPECT_NE(printer->_rgs._groups[1].ToString().find(bam2ExpectedHeader), std::string::npos);

//     EXPECT_EQ(printer->_pgs._groups.size(), 3);
// }
