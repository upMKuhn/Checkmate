#pragma once
#include "stdafx.h"
#include <format.h>
class Turtle {
public:

	virtual void PenUp() = 0;
	virtual void PenDown() = 0;
	virtual void Forward(int distance) = 0;
	virtual void Turn(int degrees) = 0;
	virtual void GoTo(int x, int y) = 0;
	virtual int GetX() const = 0;
	virtual int GetY() const = 0;
};

class MockTurtle : public Turtle {
public:
	MOCK_METHOD0(PenUp, void());
	MOCK_METHOD0(PenDown, void());
	MOCK_METHOD1(Forward, void(int distance));
	MOCK_METHOD1(Turn, void(int degrees));
	MOCK_METHOD2(GoTo, void(int x, int y));
	MOCK_CONST_METHOD0(GetX, int());
	MOCK_CONST_METHOD0(GetY, int());
};


bool testfunction(Turtle &testclass)
{
	testclass.PenDown();
	testclass.PenDown();
	return true;
}

using ::testing::AtLeast;                     // #1

TEST(PainterTest, CanDrawSomething) {
	MockTurtle turtle;                          // #2
	EXPECT_CALL(turtle, PenDown())              // #3
		.Times(AtLeast(2));
	EXPECT_TRUE(testfunction(turtle));
}

TEST(FormatLibTests, strFormat)
{
	std::string s = fmt::format("{0}{1}{0}", "abra", "cad");
	EXPECT_EQ("abracadabra", s);
}
