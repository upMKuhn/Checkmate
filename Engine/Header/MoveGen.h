#pragma once
#include "stdafx.h"

namespace Checkmate {




	enum GenerationType
	{
		Captures,EnPassant, Normal, Checks, Xray
	};

class MoveGen
{

public:

	MoveGen();
	~MoveGen();
	
	MoveList* generate_all(Color us, MoveList* mvlist, Represenation& rep);

	MoveList* generate_normal(Color us, MoveList* mvlist, Represenation& rep);

	MoveList* generate_captures(Color us, MoveList* mvlist, Represenation& rep);

	//MoveList* generate_checks(Color us, MoveList* mvlist, Represenation& rep);

	
	

private:
	void init();



};

}

