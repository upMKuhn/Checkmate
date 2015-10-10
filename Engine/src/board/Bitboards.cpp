#include "stdafx.h"
namespace Checkmate {
	Bitboard SquareBB[SQUARE_NB];


		void Bitboards::init()
		{
			for (Square square = SQ_A1; square < SQUARE_NB; ++square)
			{
				SquareBB[square] = (1ULL << square);
			}
		}

		/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
		/// to be printed to standard output. Useful for debugging.

		const std::string Bitboards::pretty(Bitboard b) {

			std::string s = "+---+---+---+---+---+---+---+---+\n";

			for (Rank r = RANK_8; r >= RANK_1; --r)
			{
				for (File f = FILE_A; f <= FILE_H; ++f)
					s.append(b & make_square(f, r) ? "| X " : "|   ");

				s.append("|\n+---+---+---+---+---+---+---+---+\n");
			}

			return s;
		}

}