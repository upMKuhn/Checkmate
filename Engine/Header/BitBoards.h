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

		extern Bitboard filebb[FILE_NB];
		extern Bitboard rankbb[RANK_NB];
		extern Bitboard SquareBB[SQUARE_NB]; //Board that represents a Square inside a BB
		extern unsigned SquareDistance[SQUARE_NB][SQUARE_NB];

		extern Bitboard* RookAttack[SQUARE_NB];
		extern Bitboard RookMask[SQUARE_NB];
		extern Bitboard* BishopAttack[SQUARE_NB];
		extern Bitboard BishopMask[SQUARE_NB];
		extern Bitboard RookMagics[SQUARE_NB];
		extern Bitboard BishopMagics[SQUARE_NB];
		extern unsigned RookShifts[SQUARE_NB];
		extern unsigned BishopShifts[SQUARE_NB];
		extern Bitboard stepAttacks[PIECE_NB][SQUARE_NB];
		
		extern Bitboard RookTable[0x19000];  // To store rook attacks
		extern Bitboard BishopTable[0x1480];

		
		inline Bitboard operator&(Bitboard b, Square s) {
			return b & 1ULL << s;
		}

		inline Bitboard operator|(Bitboard b, Square s) {
			return b | 1ULL << s;
		}

		inline Bitboard operator^(Bitboard b, Square s) {
			return b ^ 1ULL << s;
		}

		inline Bitboard& operator|=(Bitboard& b, Square s) {
			return b |= 1ULL << s;
		}

		inline Bitboard& operator^=(Bitboard& b, Square s) {
			return b ^= 1ULL << s;
		}

		inline bool more_than_one(Bitboard b) {
			return b & (b - 1);
		}

		/// rank_bb() and file_bb() return a bitboard representing all the squares on
		/// the given file or rank.

		inline Bitboard rank_bb(Rank r) {
			return rankbb[r];
		}

		inline Bitboard rank_bb(Square s) {
			return rankbb[rank_of(s)];
		}

		inline Bitboard file_bb(File f) {
			return filebb[f];
		}

		inline Bitboard file_bb(Square s) {
			return filebb[file_of(s)];
		}

		template<typename T> inline int distance(T x, T y) { return x < y ? y - x : x - y; }
		template<> inline int distance<Square>(Square x, Square y) { return SquareDistance[x][y]; }

		template<typename T1, typename T2> inline int distance(T2 x, T2 y);
		template<> inline int distance<File>(Square x, Square y) { return distance(file_of(x), file_of(y)); }
		template<> inline int distance<Rank>(Square x, Square y) { return distance(rank_of(x), rank_of(y)); }

		
		
		template<PieceType Pt>
		FORCE_INLINE unsigned magic_index(Square s, Bitboard occupied) 
		{
			Bitboard* const Masks = Pt == ROOK ? RookMask : BishopMask;
			Bitboard* const Magics = Pt == ROOK ? RookMagics : BishopMagics;
			unsigned* const Shifts = Pt == ROOK ? RookShifts : BishopShifts;

			if (HasPext)
				return unsigned(pext(occupied, Masks[s]));

			if (Is64Bit)
				return unsigned(((occupied & Masks[s]) * Magics[s]) >> Shifts[s]);

			unsigned lo = unsigned(occupied) & unsigned(Masks[s]);
			unsigned hi = unsigned(occupied >> 32) & unsigned(Masks[s] >> 32);
			return (lo * unsigned(Magics[s]) ^ hi * unsigned(Magics[s] >> 32)) >> Shifts[s];
		}
		
		inline Bitboard shift_bb(Bitboard b,Square Delta) {
			return  Delta == DELTA_N ? b << 8 : Delta == DELTA_S ? b >> 8
				: Delta == DELTA_NN ? b << 16 : Delta == DELTA_SS ? b >> 16
				: Delta == DELTA_NE ? (b & ~FileHBB) << 9 : Delta == DELTA_SE ? (b & ~FileHBB) >> 7
				: Delta == DELTA_NW ? (b & ~FileABB) << 7 : Delta == DELTA_SW ? (b & ~FileABB) >> 9
				: Delta == DELTA_E ? (b & ~FileHBB) << 1 : Delta == DELTA_W ? (b & ~FileABB) >> 1
				: 0ULL;
		}

		namespace Bitboards {

			inline Bitboard generatePawnAttack(Square sqr, Color c)
			{
				return c == WHITE ?
					shift_bb(SquareBB[sqr], DELTA_NE) | shift_bb(SquareBB[sqr], DELTA_NW) :
					shift_bb(SquareBB[sqr], DELTA_SE) | shift_bb(SquareBB[sqr], DELTA_SW);
			}

			inline Bitboard generateKingAttack(Square sqr)
			{
				return 	shift_bb(SquareBB[sqr], DELTA_NE) | shift_bb(SquareBB[sqr], DELTA_NW) |
					shift_bb(SquareBB[sqr], DELTA_SE) | shift_bb(SquareBB[sqr], DELTA_SW) |
					shift_bb(SquareBB[sqr], DELTA_S) | shift_bb(SquareBB[sqr], DELTA_N) |
					shift_bb(SquareBB[sqr], DELTA_W) | shift_bb(SquareBB[sqr], DELTA_E);
			}

			inline Bitboard generateKnightAttack(Square sqr)
			{
				const Bitboard leftIgnore = 0xc0c0c0c0c0c0c0c0ULL;
				const Bitboard rightIgnore = 0x303030303030303ULL;

				Bitboard right = (sqr + 17 < 63 ? (1ULL << (sqr + 17)) : 0) |
					(sqr + 10 < 63 ? (1ULL << (sqr + 10)) : 0) |
					(sqr - 15 > 0 ? 1ULL << (sqr - 15) : 0) |
					(sqr - 6 > 0 ? 1ULL << (sqr - 6) : 0);

				Bitboard left = (sqr + 15 < 63 ? (1ULL << (sqr + 15)) : 0) |
					(sqr + 6 < 63 ? (1ULL << (sqr + 6)) : 0) |
					(sqr - 17 > 0 ? 1ULL << (sqr - 17) : 0) |
					(sqr - 10 > 0 ? 1ULL << (sqr - 10) : 0);

				return (left & ~leftIgnore) | (right & ~rightIgnore);
			}

			inline Bitboard generateRookAttack(Square sqr)
			{
				return (filebb[file_of(sqr)] | rankbb[rank_of(sqr)]) & ~SquareBB[sqr];
			}

			inline Bitboard pawn_moves(Square sqr, Color c, Bitboard occupancy)
			{
				Bitboard b = shift_bb(SquareBB[sqr], pawn_push(c));
				return pawn_canjump(c, sqr) ? b | shift_bb(SquareBB[sqr], pawn_push(c) * 2) : b;
			}

			inline Bitboard sliding_attack(Square square, Square delta[], int len, Bitboard occupency)
			{
				Bitboard attack = 0;
				Bitboard lastSqr = SQ_NONE;
				for (int i = 0; i < len; ++i)
				{
					for (Square s = square; s >= SQ_A1 && s <= SQ_H8 && lastSqr; s += delta[i])
					{
						lastSqr = shift_bb(SquareBB[s], delta[i]);
						attack |= lastSqr;

						if (occupency & lastSqr)
							break;
					}
					lastSqr = SQ_NONE;
				}
				return attack;
			}

		}

		inline Bitboard moves_for(PieceType Pt,Square from, Color us, Bitboard occupied)
		{
			using namespace Bitboards;
			return Pt == ROOK ? RookAttack[from][magic_index<ROOK>(from, occupied)]
				: Pt == BISHOP ? BishopAttack[from][magic_index<BISHOP>(from, occupied)]
				: Pt == QUEEN ? moves_for(ROOK,from, us, occupied) | moves_for(BISHOP,from, us, occupied)
				: Pt == PAWN ? (stepAttacks[make_piece(us, Pt)][from] & occupied) | pawn_moves(from,us,occupied)
				: stepAttacks[make_piece(us,Pt)][from];
		}
		
		inline Bitboard attacks_from(PieceType Pt,Square from,Color us, Bitboard occupied)
		{
			return moves_for(Pt,from,us,occupied) & occupied;
		}


		namespace Bitboards {
			void init();
			void initAttacks();
			const std::string print(Bitboard b);
		}

		
		

		

/// lsb() and msb() return the least/most significant bit in a non-zero bitboard

#ifdef USE_BSFQ

#  if defined(_MSC_VER) && !defined(__INTEL_COMPILER)

		FORCE_INLINE Square lsb(Bitboard b) {
			unsigned long idx;
			_BitScanForward64(&idx, b);
			return (Square)idx;
		}

		FORCE_INLINE Square msb(Bitboard b) {
			unsigned long idx;
			_BitScanReverse64(&idx, b);
			return (Square)idx;
		}

#  elif defined(__arm__)

		FORCE_INLINE int lsb32(uint32_t v) {
			__asm__("rbit %0, %1" : "=r"(v) : "r"(v));
			return __builtin_clz(v);
		}

		FORCE_INLINE Square msb(Bitboard b) {
			return (Square)(63 - __builtin_clzll(b));
		}

		FORCE_INLINE Square lsb(Bitboard b) {
			return (Square)(uint32_t(b) ? lsb32(uint32_t(b)) : 32 + lsb32(uint32_t(b >> 32)));
		}

#  else // Assumed gcc or compatible compiler

		FORCE_INLINE Square lsb(Bitboard b) { // Assembly code by Heinz van Saanen
			Bitboard idx;
			__asm__("bsfq %1, %0": "=r"(idx) : "rm"(b));
			return (Square)idx;
		}

		FORCE_INLINE Square msb(Bitboard b) {
			Bitboard idx;
			__asm__("bsrq %1, %0": "=r"(idx) : "rm"(b));
			return (Square)idx;
		}

#  endif

#else // ifdef(USE_BSFQ)

		Square lsb(Bitboard b);
		Square msb(Bitboard b);

#endif


		/// pop_lsb() finds and clears the least significant bit in a non-zero bitboard

		FORCE_INLINE Square pop_lsb(Bitboard& b) {
			const Square s = b > 0 ?lsb(b): SQ_NONE;
			b &= ~(1ULL << s);
			return s;
		}

		FORCE_INLINE Square pop_msb(Bitboard& b) {
			const Square s = b > 0 ? msb(b) : SQ_NONE;
			b &= ~(1ULL << s);
			return s;
		}


		FORCE_INLINE unsigned  pop_count(Bitboard b) {
			int count = 0;
			while (msb(b))
			{
				count++;
				b &= b - 1;
			}
			return count;
		}

		/// frontmost_sq() and backmost_sq() return the square corresponding to the
		/// most/least advanced bit relative to the given color.

		inline Square frontmost_sq(Color c, Bitboard b) { return c == WHITE ? msb(b) : lsb(b); }
		inline Square  backmost_sq(Color c, Bitboard b) { return c == WHITE ? lsb(b) : msb(b); }

}