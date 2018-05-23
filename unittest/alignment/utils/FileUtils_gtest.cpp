/*
 * =====================================================================================
 *
 *       Filename:  FileUtils_gtest.cpp
 *
 *    Description:  Test alignment/utils/FileUtils.hpp
 *
 *        Version:  1.0
 *        Created:  10/29/2012 05:20:43 PM
 *       Revision:  08/20/2014
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include <gtest/gtest.h>

#include <alignment/utils/FileUtils.hpp>

std::string nonexistfile = "/nonexistingdir/nonexistingfile";
std::string readablefile = "/bin/ls";
std::string writeablefile = "/tmp/writabletmpfile";
std::string expected_errmsg = "^.+$";

TEST(FILEUTILS, CriticalOpenRead)
{
    std::ifstream ifs;
    EXPECT_EXIT(CriticalOpenRead(nonexistfile, ifs, std::ios::in), ::testing::ExitedWithCode(1),
                expected_errmsg);
    CriticalOpenRead(readablefile, ifs, std::ios::in);
}

TEST(FILEUTILS, OpenRead)
{
    std::ifstream ifs;
    EXPECT_EQ(OpenRead(nonexistfile, ifs, std::ios::in), 0);
    EXPECT_EQ(OpenRead(readablefile, ifs, std::ios::in), 1);
}

TEST(FILEUTILS, CriticalOpenWrite)
{
    std::ofstream ofs;
    EXPECT_EXIT(CriticalOpenWrite(nonexistfile, ofs, std::ios::out), ::testing::ExitedWithCode(1),
                expected_errmsg);
    CriticalOpenWrite(writeablefile, ofs, std::ios::out);
}

TEST(FILEUTILS, OpenWrite)
{
    std::ofstream ofs;
    EXPECT_EQ(OpenWrite(nonexistfile, ofs, std::ios::out), 0);
    EXPECT_EQ(OpenWrite(writeablefile, ofs, std::ios::out), 1);
}
