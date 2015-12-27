#pragma once
#include "stdafx.h"

namespace Checkmate {

	struct MoveInfo
	{

		MoveInfo(Color Us, Square from, PieceType mving_pt, MoveType movetype, Represenation& refRep)
			: Us(Us), movingPt(mving_pt), movetype(movetype), from(from), rep(refRep) {}

		MoveInfo(Color Us, Square from, Bitboard destionations, PieceType mving_pt, MoveType movetype, Represenation& refRep)
			: destionations(destionations), Us(Us), movingPt(mving_pt), movetype(movetype), from(from), rep(refRep) {}
		Represenation& rep;
		Bitboard destionations = 0ULL;
		PieceType movingPt = NO_PIECE_TYPE;
		PieceType promotion = NO_PIECE_TYPE;
		MoveType movetype = NORMAL;
		Square from = SQ_NONE;
		Color Us = NO_COLOR;
	};

}
