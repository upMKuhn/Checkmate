#pragma once
#include "Stdafx.h"



#define DELETE_TABLE( ptr ) \
if (ptr != NULL)            \
{                           \
    delete[] ptr;           \
    ptr = NULL;             \
}



#define DELETE_PTR( ptr ) \
if (ptr != NULL)            \
{                           \
    delete ptr;           \
    ptr = NULL;             \
}