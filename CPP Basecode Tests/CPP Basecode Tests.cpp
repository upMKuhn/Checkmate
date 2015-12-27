// CPP Basecode Tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



int main(int argc, char* argv[]) {
	::testing::GTEST_FLAG(throw_on_failure) = false;
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

