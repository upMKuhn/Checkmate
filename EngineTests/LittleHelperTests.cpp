#include "stdafx.h"
#include <LittleHelper.h>


TEST(LittleHelperTests,StringFormat)
{
	using namespace Checkmate;
	std::string str = "Hello {0}{1}";
	
	//EXPECT_EQ("Hello World!", strFormat(str, { std::to_string("World"),"!" },2));
}