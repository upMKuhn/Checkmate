#pragma once
#include "Stdafx.h"



#define DELETE_TABLE( ptr ) \
if (ptr != NULL)            \
{                           \
    delete[] ptr;           \
    ptr = NULL;             \
}