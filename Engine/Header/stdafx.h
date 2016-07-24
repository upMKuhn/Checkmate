#pragma once
#ifndef std_H
	#define std_H
	#include <cassert>
	#include <cctype>
	#include <climits>
	#include <cstdlib>
	#include <string>
	#include <iostream>
	#include <sstream>
	#include <vector>
	#include <string>
	#include <ostream>
	#include <fstream>
	#include <iomanip>
	#include <stack>
	#include <math.h>
	#include <queue>

	#include <boost\lexical_cast.hpp>

	#include "CppBaseCode.h"

	#include "platform.h"
	#include "EngineMacros.h"
	#include "types.h"	

	#include "misc.h"
	#include "bitcount.h"
		

	//Project Refs
	#include "CustomList.h"
	#include "LittleHelper.h"
	#include "piecePlacement.h"
	#include "BitBoards.h"
	
	#include "BoardParser.h"
	#include "FEN.h"
	#include "represenation.h"
		
	#include "MoveInfo.h"
	#include "MoveListBase.h"
	#include "ChachedMoveList.h"
	#include "SimpleMoveList.h"
	#include "MoveList.h"

	#include "MoveGenInfo.hpp"	
	#include "BaseEvaluation.h"
	#include "EarlyEvaluation.h"
	#include "MidGameEvaluation.h"
	#include "Evaluation.h"
	#include "Zorbist.h"
	#include "MoveGen.h"

	#include "Transposition.h"
	#include "HashedStates.h"
	#include "SearchBase.h"
	#include "PrincipleVariationSearch.h"

	#include "UCI.h"


#endif
