#include "stdafx.h"
#include "LittleHelper.h"

namespace Checkmate {
	std::string& strFormat(std::string& str, std::string inserts[], int length)
	{
		//I would use Array length, but i don't think it works with objects
		for (int insertLen = 0; insertLen < length; insertLen++)
		{
			std::stringstream ss;
			ss << "{" << ::std::to_string(insertLen) << "}";
			str.replace(0, ss.str().length(), ss.str());
		}
		return str;
	}
}


