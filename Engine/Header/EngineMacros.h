#pragma once
#include "stdafx.h"

#define ARRAY_LENGHT(x) sizeof(x) / sizeof(x[0])

#define ENABLE_CHECKS true

#define MAKE_ZORBIST true | ENABLE_CHECKS


#if ENABLE_CHECKS
	#define CHECKS_ENABLED(x) x
#else
	#define CHECKS_ENABLED(x) 
#endif