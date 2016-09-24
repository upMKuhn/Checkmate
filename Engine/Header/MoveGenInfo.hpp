#pragma once
#include "stdafx.h"

namespace Checkmate{

struct Engine_API MoveGenInfo
{
	MoveGenInfo()
	{
		memset(pinned_from, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(attacks_from, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(xray_from, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(checker_from, 0ULL, 64 * sizeof(Bitboard) * 2);

		memset(pinned_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(attacks_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(xray_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(checker_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(attackableSquares, 0ULL, 65 * sizeof(Bitboard) * 2);

		protectPieces[WHITE] = 0ULL;
		protectPieces[BLACK] = 0ULL;
		move_list = new SimpleMoveList();
	}

	~MoveGenInfo()
	{
		SAFE_DELETE(move_list);
	}

	inline bool is_pinned(Color us, Square s)
	{
		return pinned_from[us][s] > 0;
	}

	inline void OnGeneratingNewMoves(Move m)
	{
		memset(pinned_from, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(attacks_from, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(xray_from, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(checker_from, 0ULL, 64 * sizeof(Bitboard) * 2);

		memset(pinned_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(attacks_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(xray_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(checker_to, 0ULL, 64 * sizeof(Bitboard) * 2);
		memset(attackableSquares, 0ULL, 65 * sizeof(Bitboard) * 2);

		protectPieces[WHITE] = 0ULL;
		protectPieces[BLACK] = 0ULL;
	}


	bool isCheck = false;
	MoveListBase* move_list;

	Bitboard protectPieces[NO_COLOR];
	Bitboard attackableSquares[NO_COLOR][SQUARE_NB+1];

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