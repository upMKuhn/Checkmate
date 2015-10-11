#pragma once
#include "stdafx.h"
#ifndef SSTREAM_H
#define SSTREAM_H
#include <sstream>
#endif

namespace Checkmate
{
	std::string& strFormat(std::string& str, char inserts[][], int length);
}
