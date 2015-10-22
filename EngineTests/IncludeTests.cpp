#include "stdafx.h"
#include "IncludeTests.h"

TEST(GoogleMockWorks, TestForLinkerErrors) {
	MockTurtle turtle;                          // #2
	EXPECT_CALL(turtle, PenDown())              // #3
		.Times(AtLeast(2));
	EXPECT_TRUE(testfunction(turtle));
}

TEST(FormatLib, TestForLinkerErrors)
{
	std::string s = fmt::format("{0}{1}{0}", "abra", "cad");
	EXPECT_EQ("abracadabra", s);
}




