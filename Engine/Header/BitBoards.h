#pragma once
#include "stdafx.h"
namespace Checkmate {


		const Bitboard DarkSquares = 0xAA55AA55AA55AA55ULL;

		const Bitboard FileABB = 0x0101010101010101ULL;
		const Bitboard FileBBB = FileABB << 1;
		const Bitboard FileCBB = FileABB << 2;
		const Bitboard FileDBB = FileABB << 3;
		const Bitboard FileEBB = FileABB << 4;
		const Bitboard FileFBB = FileABB << 5;
		const Bitboard FileGBB = FileABB << 6;
		const Bitboard FileHBB = FileABB << 7;

		const Bitboard Rank1BB = 0xFF;
		const Bitboard Rank2BB = Rank1BB << (8 * 1);
		const Bitboard Rank3BB = Rank1BB << (8 * 2);
		const Bitboard Rank4BB = Rank1BB << (8 * 3);
		const Bitboard Rank5BB = Rank1BB << (8 * 4);
		const Bitboard Rank6BB = Rank1BB << (8 * 5);
		const Bitboard Rank7BB = Rank1BB << (8 * 6);
		const Bitboard Rank8BB = Rank1BB << (8 * 7);

		extern Bitboard SquareBB[SQUARE_NB]; //Board that represents a Square inside a BB
		namespace Bitboards {
			void init();
		}
}