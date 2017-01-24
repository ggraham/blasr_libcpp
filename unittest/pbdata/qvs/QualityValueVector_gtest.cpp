/*
 * =====================================================================================
 *
 *       Filename:  QualityValueVector_gtest.cpp
 *
 *    Description:  Test pbdata/qvs/QualityValueVector.hpp
 *
 *        Version:  1.0
 *        Created:  03/28/2015 05:21:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */
#include "gtest/gtest.h"
#include "qvs/QualityValue.hpp"
#include "qvs/QualityValueVector.hpp"

using namespace std;

const string qvstr = "!#$%0123:;ABab{|}~";
vector<uint8_t> data = {0, 2, 3, 4, 15, 16, 17, 18, 25, 26, 32, 33, 64, 65, 90, 91, 92, 93};

vector<uint8_t> data2 = {1, 1, 17, 18, 25, 26, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

TEST(QualityValueVectorTest, Copy)
{
    EXPECT_EQ(qvstr.size(), data.size());

    QualityValueVector<QualityValue> qual;
    EXPECT_TRUE(qual.Empty());

    // Copy qvs from a string
    qual.Copy(qvstr);
    EXPECT_FALSE(qual.Empty());
    for (size_t i = 0; i < qvstr.size(); i++) {
        EXPECT_EQ(static_cast<uint8_t>(qual.data[i]), data[i]);
    }
}

TEST(QualityValueVectorTest, ToString)
{
    QualityValueVector<QualityValue> qual;
    qual.Copy(qvstr);

    // Test ToString()
    EXPECT_EQ(qual.ToString(), qvstr);

    EXPECT_EQ(static_cast<size_t>(qual.Length()), qvstr.size());
}

TEST(QualityValueVetorTest, Fill)
{
    QualityValueVector<QualityValue> qual;
    qual.Copy(qvstr);

    // First fill with all 1s
    QualityValueVector<QualityValue> qual2;
    EXPECT_TRUE(qual2.Empty());

    qual2.Allocate(qvstr.size());
    qual2.Fill(1);

    EXPECT_FALSE(qual2.Empty());
    EXPECT_EQ(qual2.Length(), qvstr.size());

    for (size_t i = 0; i < qual2.Length(); i++) {
        EXPECT_EQ(qual2.data[i], 1);
    }

    // Create qual3
    QualityValueVector<QualityValue> qual3;
    qual3.Copy(qvstr.substr(5, 5));  //qual3.data[0..5] = {16,17,18,25,26}
    EXPECT_EQ(qual3.Length(), 5);

    // Then fill qual2.data[2, ..., 6) with data from qual3.data[1, ...,5)
    qual2.Fill(2, 4, qual3, 1);

    for (size_t i = 0; i < qual2.Length(); i++) {
        EXPECT_EQ(qual2.data[i], data2[i]);
    }
}
