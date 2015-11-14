#include "stdafx.h"
using namespace Checkmate;

		

		const Key PieceKey[] = { 0x0ULL, 0x02564561245636ULL,0x123556113053400ULL,0x4132156455641ULL, 0x054321561653123ULL, 0x3652436541265ULL, 0x0564897345221ULL,
		0x0ULL, 0x983216575121385ULL, 0x7890423890785ULL, 0x25155661565ULL, 0x15464545601ABC5ULL, 0xA89121243342123ULL, 0x068312131234562ULL };

		Key SquareKey[SQUARE_NB];


		void Zorbist::init()
		{
			for (Square s = SQ_A1; s < SQUARE_NB; ++s)
			{
				PRNG rng((rank_of(s)+1 * 0xE4591) ^ file_of(s)+1 * 0xA82F31);
				SquareKey[s] = rng.sparse_rand<uint64_t>();
			}
		}

		Key Checkmate::Zorbist::make_zorbist(Square pieceList[NO_COLOR][PIECE_TYPE_NB][16])
		{
			#if MAKE_ZORBIST
				Key key = 0;
				for (Color c = WHITE; c <= BLACK; ++c) {
					for(PieceType pt = PAWN; pt < PIECE_TYPE_NB; ++pt)
					{
						for each(Square s in pieceList[c][pt])
						{
							if (s == SQ_NONE) { break; }
							key ^= SquareKey[s];
							key ^= PieceKey[make_piece(c, pt)];
						}
					}
				}
				return key;
			#endif
			return 0;
		}


		

