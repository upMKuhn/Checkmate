#include "BitBoards.h"

namespace Checkmate {
	Bitboard SquareBB[SQUARE_NB];
	namespace Bitboards {

		void init()
		{
			for (Square square = SQ_A1; square < SQUARE_NB; ++square)
			{
				SquareBB[square] = 1 << square;
			}
		}

}}