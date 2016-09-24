#pragma once
#include "Stdafx.h"

#ifdef _WIN32
#ifdef Base_CODE_DLL
#define Base_API __declspec(dllexport)
#else
#define Base_API __declspec(dllimport)
#endif
#else
#define SP_API
#endif 


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