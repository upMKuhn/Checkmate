#pragma once
#include "stdafx.h"
#include <format.h>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <functional>
#include <boost\algorithm\algorithm.hpp>
#include <boost\tokenizer.hpp>
#include <boost\thread.hpp>
#include <boost\log\trivial.hpp>
#include <boost\log\utility\setup.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost\algorithm\string.hpp>
#include <vector>

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

