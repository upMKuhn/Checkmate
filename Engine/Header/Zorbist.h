#pragma once
#include "stdafx.h"

namespace Checkmate
{
	namespace Zorbist
	{

		const Key PieceKey[] = { 0x0ULL, 0x02564561245636ULL,0x123556113053400ULL,0x4132156455641ULL, 0x054321561653123ULL, 0x3652436541265ULL, 0x0564897345221ULL,
			0x0ULL, 0x983216575121385ULL, 0x7890423890785ULL, 0x25155661565ULL, 0x15464545601ABC5ULL, 0xA89121243342123ULL, 0x068312131234562ULL };

		extern Key SquareKeys[SQUARE_NB];



		#define PieceZorbist(sqr, piece)  (Zorbist::PieceKey[piece] ^ Zorbist::SquareKeys[sqr])
		#define MoveZorbist(from, to)  (Zorbist::SquareKeys[from] ^ Zorbist::SquareKeys[to])

		void init();

		Key make_zorbist(Represenation* rep, BoardState* state);

	}
}