// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <gmock\gmock.h>
#include <gtest/gtest.h>
#include <Engine_inc.h>

#include "TestingTools.h"


#undef ENABLE_CHECKS
#define ENABLE_CHECKS true