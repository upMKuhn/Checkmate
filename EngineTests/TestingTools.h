#pragma once
#include "stdafx.h"


template<class T>
void EXPECT_ARRAY_EQ(T ar1[],T ar2[])
{
	EXPECT_TRUE(ARRAY_LENGHT(ar1) == ARRAY_LENGHT(ar2));
	for (int i = 0; i < ARRAY_LENGHT(ar1); i++)
	{
		EXPECT_EQ(ar1[i], ar2[i]);
	}
}