/*
 * =====================================================================================
 *
 *       Filename:  utils_gtest.cpp
 *
 *    Description:  Test pbdata/utils.hpp
 *
 *        Version:  1.0
 *        Created:  08/19/2014 05:22:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include <pbdata/utils.hpp>

TEST(UTILS, CrucialOpen)
{
    std::ifstream ifs;
    std::string filename = "/nonexistingdir/nonexistingfile";
    //Expected behavour: exit with a message.
    EXPECT_EXIT(CrucialOpen(filename, ifs, std::ios::in), ::testing::ExitedWithCode(1), "");
}

TEST(UTILS, CeilOfFraction)
{
    EXPECT_EQ(CeilOfFraction<int>(7, 100), 1);
    EXPECT_EQ(CeilOfFraction<int>(100, 7), 15);
    EXPECT_EQ(CeilOfFraction<int>(100, 99), 2);
    EXPECT_EQ(CeilOfFraction<int>(100, 5), 20);
}
