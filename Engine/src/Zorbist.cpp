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
				//contains identical
				bool test = true;
				while (test)
				{
					test = false;
					for (Square sqr = SQ_A1; sqr < s; ++sqr)
					{
						if (SquareKey[s] == SquareKey[sqr])
						{
							test = true;
							SquareKey[s] ^= rng.sparse_rand<uint64_t>();
						}
					}
				}
			}
		}

		Key Zorbist::make_zorbist(Represenation* rep)
		{
			Key key = 0;
			#if MAKE_ZORBIST
				Square s = SQ_NONE;
				Bitboard bb = rep->colorbb[BLACK] | rep->colorbb[WHITE];
				while (s = pop_lsb(bb), is_ok(s))
				{
					key ^= SquareKey[s];
					key ^= PieceKey[rep->piece_on(s)];
				}
			#endif
			return key;
		}


		

