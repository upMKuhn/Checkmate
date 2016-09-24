#include "stdafx.h"
using namespace Checkmate;

	Bitboard Zorbist::SquareKeys[SQUARE_NB];

	void Zorbist::init()
		{
			
			for (Square s = SQ_A1; s < SQUARE_NB; ++s)
			{
				PRNG rng((rank_of(s)+1 * 0xE4591) ^ file_of(s)+1 * 0xA82F31);
				SquareKeys[s] = rng.sparse_rand<uint64_t>();
				//contains identical
				bool test = true;
				while (test)
				{
					test = false;
					for (Square sqr = SQ_A1; sqr < s; ++sqr)
					{
						if (SquareKeys[s] == SquareKeys[sqr])
						{
							test = true;
							SquareKeys[s] ^= rng.sparse_rand<uint64_t>();
						}
					}
				}
			}
		}

	Key Zorbist::make_zorbist(Represenation* rep, BoardState* state)
		{
			Key key = 0;
			#if MAKE_ZORBIST
				Square s = SQ_NONE;
				Bitboard bb = rep->typebb[ALL_PIECES];
				while (s = pop_lsb(bb), bb > 0)
					key = key ^ SquareKeys[s] ^ PieceKey[rep->board[s]];
			#endif
			rep->zorbist = key;
			state->zorbist = key;
			return key;
		}


		



		

