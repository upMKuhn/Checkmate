// stdafx.cpp : source file that includes just the standard includes
// EngineTests.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


int main(int argc, char* argv[]) {

	::testing::GTEST_FLAG(throw_on_failure) = true;
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}