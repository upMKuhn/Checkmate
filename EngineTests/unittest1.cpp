#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


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


namespace EngineTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestFunc_Pass6_DeductOne)
		{
			Assert::AreEqual(5 , explore::testFunc(6));
		}

	};
}