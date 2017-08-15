/*
 * =====================================================================================
 *
 *       Filename:  VectorUtils_gtest.cpp
 *
 *    Description:  Test pbdata/VectorUtils.hpp
 *
 *        Version:  1.0
 *        Created:  01/17/2013 06:01:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yuan Li (yli), yli@pacificbiosciences.com
 *        Company:  Pacific Biosciences
 *
 * =====================================================================================
 */

#include <vector>

#include "VectorUtils.hpp"
#include "gtest/gtest.h"

// Test ClearMemory(std::vector<T> vt)
TEST(VectorUtils, ClearMemory)
{
    std::vector<int> vi;
    vi.push_back(1);

    unsigned size = 1000000;
    vi.reserve(size);
    EXPECT_EQ(vi.size(), 1u);
    EXPECT_EQ(vi.capacity(), size);

    ClearMemory(vi);
    EXPECT_EQ(vi.size(), 0u);
    EXPECT_EQ(vi.capacity(), 0u);
}
