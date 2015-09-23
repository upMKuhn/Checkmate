#include "stdafx.h"


namespace explore {

	int testFunc(int a)
	{
		if (a > 5)
		{
			a -= 1;
		}
		else
		{
			a++;
		}
		return a;

	}

}

using namespace explore;

TEST(Testaasd, blaTest)
{
	EXPECT_EQ(5, testFunc(6));
}


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

