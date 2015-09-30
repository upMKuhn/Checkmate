// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <gtest\gtest.h>


TEST(TESTSTUFF, BLA)
{
	EXPECT_EQ(1, 1);
}


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

