#pragma once
#include "stdafx.h"

namespace Checkmate{

struct MoveGenInfo
{
	MoveGenInfo()
	{
		for (Color c = WHITE; c < NO_COLOR; ++c)
		{
			std::fill_n(pinned_from[c], SQUARE_NB, 0ULL);
			std::fill_n(attacks_from[c], SQUARE_NB, 0ULL);
			std::fill_n(xray_from[c], SQUARE_NB, 0ULL);
			std::fill_n(checker_from[c], SQUARE_NB, 0ULL);

			std::fill_n(pinned_to[c], SQUARE_NB, 0ULL);
			std::fill_n(attacks_to[c], SQUARE_NB, 0ULL);
			std::fill_n(xray_to[c], SQUARE_NB, 0ULL);
			std::fill_n(checker_to[c], SQUARE_NB, 0ULL);

			protectPieces[c] = 0ULL;
			attackableSquares[c] = 0ULL;
		}
		move_list = CreateNewMoveList();
	}

	inline bool is_pinned(Color us, Square s)
	{
		return pinned_from[us][s] > 0;
	}

	inline void OnGeneratingNewMoves(Move m)
	{
		if (m != MOVE_NONE)
			move_list->OnGeneratingNewMoves(m);
	}


	bool isCheck = false;
	MoveListBase* move_list;

	Bitboard protectPieces[NO_COLOR];
	Bitboard attackableSquares[NO_COLOR];

	//Usefull information collected through out!
	Bitboard pinned_from[NO_COLOR][SQUARE_NB];
	Bitboard blocker_from[NO_COLOR][SQUARE_NB];
	Bitboard attacks_from[NO_COLOR][SQUARE_NB];
	Bitboard xray_from[NO_COLOR][SQUARE_NB];
	Bitboard checker_from[NO_COLOR][SQUARE_NB];

	Bitboard blocker_to[NO_COLOR][SQUARE_NB];
	Bitboard pinned_to[NO_COLOR][SQUARE_NB];
	Bitboard attacks_to[NO_COLOR][SQUARE_NB];
	Bitboard xray_to[NO_COLOR][SQUARE_NB];
	Bitboard checker_to[NO_COLOR][SQUARE_NB];
};


}