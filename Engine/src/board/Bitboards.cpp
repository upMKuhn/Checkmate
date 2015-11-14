#include "stdafx.h"
namespace Checkmate {

	Bitboard filebb[FILE_NB] = { FileABB,FileBBB ,FileCBB ,FileDBB ,FileEBB ,FileFBB ,FileGBB ,FileHBB };
	Bitboard rankbb[RANK_NB] = { Rank1BB ,Rank2BB,Rank3BB,Rank4BB,Rank5BB,Rank6BB,Rank7BB,Rank8BB };
	Bitboard SquareBB[SQUARE_NB]; //Board that represents a Square inside a BB

	Bitboard* RookAttack[SQUARE_NB];
	Bitboard* BishopAttack[SQUARE_NB];
	Bitboard RookMask[SQUARE_NB];
	Bitboard BishopMask[SQUARE_NB];
	Bitboard RookMagics[SQUARE_NB];
	Bitboard BishopMagics[SQUARE_NB];
	unsigned RookShifts[SQUARE_NB];
	unsigned BishopShifts[SQUARE_NB];
	unsigned SquareDistance[SQUARE_NB][SQUARE_NB];
	Bitboard stepAttacks[PIECE_NB][SQUARE_NB];


	Bitboard RookTable[0x19000];  // To store rook attacks
	Bitboard BishopTable[0x1480];

	typedef unsigned (Fn)(Square, Bitboard);

	void init_magics(Bitboard table[], Bitboard* attacks[], Bitboard magics[], Bitboard masks[], unsigned shifts[], Square deltas[], Fn index);

	namespace Bitboards {
		void Bitboards::init()
		{

			for (Square square = SQ_A1; square < SQUARE_NB; ++square)
			{
				SquareBB[square] = (1ULL << square);
				BishopAttack[square] = 0; RookAttack[square] = 0;
				RookMask[square] = 0; BishopMask[square] = 0;
			}
			initAttacks();

		}

		void Bitboards::initAttacks()
		{
			Bitboard mask[SQUARE_NB];

			//isRook = 1 -> delta 1
			Square deltas[][RANK_NB] = {
				{DELTA_NE,DELTA_NW, DELTA_SE, DELTA_SW},
				{DELTA_N,DELTA_S, DELTA_W, DELTA_E}
			};

			int size;
			Bitboard occupancy[4096], reference[4096], edges, b;
			b = size = 0;


			for (Square sqr = SQ_A1; sqr < SQUARE_NB; ++sqr)
			{

				Bitboard edges = ((FileABB | FileHBB) & ~filebb[file_of(sqr)]) | ((Rank1BB | Rank8BB) & ~rankbb[rank_of(sqr)]);

				stepAttacks[W_KING][sqr] = generateKingAttack(sqr);
				stepAttacks[W_KNIGHT][sqr] = generateKnightAttack(sqr);
				stepAttacks[W_PAWN][sqr] = generatePawnAttack(sqr, WHITE);
				stepAttacks[B_PAWN][sqr] = generatePawnAttack(sqr, BLACK);

				stepAttacks[B_KING][sqr] = stepAttacks[W_KING][sqr];
				stepAttacks[B_KNIGHT][sqr] = stepAttacks[W_KNIGHT][sqr];

				edges = ((FileABB | FileHBB) & ~filebb[file_of(sqr)]) | ((Rank1BB | Rank8BB) & ~rankbb[rank_of(sqr)]);

				for (Square s2 = SQ_A1; s2 < SQUARE_NB; ++s2) {
					SquareDistance[sqr][s2] = std::fmax(distance<File>(sqr, s2), distance<Rank>(sqr, s2));
				}
			}

			init_magics(RookTable, RookAttack, RookMagics, RookMask, RookShifts, deltas[1], magic_index<ROOK>);
			init_magics(BishopTable, BishopAttack, BishopMagics, BishopMask, BishopShifts, deltas[0], magic_index<BISHOP>);

		}

		/// Bitboards::pretty() returns an ASCII representation of a bitboard suitable
		/// to be printed to standard output. Useful for debugging.
		const std::string Bitboards::print(Bitboard b) {

			std::string s = "-+---+---+---+---+---+---+---+---+\n";

			for (Rank r = RANK_8; r >= RANK_1; --r)
			{
				for (File f = FILE_A; f <= FILE_H; ++f)
				{
					s.append(b & (1ULL << make_square(f, r)) ? "| X " : "|   ");
				}
				s.append("|\n+---+---+---+---+---+---+---+---+\n");
			}

			return s;
		}
	}

	void init_magics(Bitboard table[], Bitboard* attacks[], Bitboard magics[],
		Bitboard masks[], unsigned shifts[], Square deltas[], Fn index) {
		using namespace Bitboards;

		int seeds[][RANK_NB] = { { 8977, 44560, 54343, 38998,  5731, 95205, 104912, 17020 },
		{ 728, 10316, 55013, 32803, 12281, 15100,  16645,   255 } };

		Bitboard occupancy[4096], reference[4096], edges, b;
		int i, size;

		// attacks[s] is a pointer to the beginning of the attacks table for square 's'
		attacks[SQ_A1] = table;

		for (Square s = SQ_A1; s <= SQ_H8; ++s)
		{
			// Board edges are not considered in the relevant occupancies
			edges = ((Rank1BB | Rank8BB) & ~rank_bb(s)) | ((FileABB | FileHBB) & ~file_bb(s));

			// Given a square 's', the mask is the bitboard of sliding attacks from
			// 's' computed on an empty board. The index must be big enough to contain
			// all the attacks for each possible subset of the mask and so is 2 power
			// the number of 1s of the mask. Hence we deduce the size of the shift to
			// apply to the 64 or 32 bits word to get the index.
			masks[s] = sliding_attack(s, deltas, 4, 0) & ~edges;
			shifts[s] = (Is64Bit ? 64 : 32) - popcount<Max15>(masks[s]);

			// Use Carry-Rippler trick to enumerate all subsets of masks[s] and
			// store the corresponding sliding attack bitboard in reference[].
			b = size = 0;
			do {
				occupancy[size] = b;
				reference[size] = sliding_attack(s, deltas, 4, b);

				if (HasPext)
					attacks[s][pext(b, masks[s])] = reference[size];

				size++;
				b = (b - masks[s]) & masks[s];
			} while (b);

			// Set the offset for the table of the next square. We have individual
			// table sizes for each square with "Fancy Magic Bitboards".
			if (s < SQ_H8)
				attacks[s + 1] = attacks[s] + size;

			if (HasPext)
				continue;

			PRNG rng(seeds[Is64Bit][rank_of(s)]);

			// Find a magic for square 's' picking up an (almost) random number
			// until we find the one that passes the verification test.
			do {
				do
					magics[s] = rng.sparse_rand<Bitboard>();
				while (popcount<Max15>((magics[s] * masks[s]) >> 56) < 6);

				std::memset(attacks[s], 0, size * sizeof(Bitboard));

				// A good magic must map every possible occupancy to an index that
				// looks up the correct sliding attack in the attacks[s] database.
				// Note that we build up the database for square 's' as a side
				// effect of verifying the magic.
				for (i = 0; i < size; ++i)
				{
					Bitboard& attack = attacks[s][index(s, occupancy[i])];

					if (attack && attack != reference[i])
						break;

					assert(reference[i]);

					attack = reference[i];
				}
			} while (i < size);
		}
	}
	
}