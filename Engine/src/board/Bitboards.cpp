#include "stdafx.h"
namespace Checkmate {
	Bitboard SquareBB[SQUARE_NB];


		void Bitboards::init()
		{
			for (Square square = SQ_A1; square < SQUARE_NB; ++square)
			{
				SquareBB[square] = 1 << square;
			}
		}

}