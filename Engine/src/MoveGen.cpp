#include "stdafx.h"


namespace Checkmate {

	MoveGen::MoveGen()
	{
	}

	MoveGen::~MoveGen()
	{
	}

	void MoveGen::init()
	{

	}

	 MoveList* MoveGen::generate_pawnMoves(Color us, MoveList* mvlist)
	{
		Bitboard our = mvlist->rep->getColorbb(us); Bitboard them = mvlist->rep->getColorbb(~us);
		Bitboard occupied = our | them;
		Bitboard move;
		Square* list = mvlist->rep->pieceList[us][PAWN];
		MoveInfo& mi = MoveInfo(our, PAWN, NORMAL, SQ_A1);
		for (int i = 0; i < 16 && list[i] != SQ_NONE; i++)
		{
			Bitboard move = 0; Square from = list[i];
			move |= shift_bb(SquareBB[from], pawn_push(us)) &  ~occupied;
			move |= move > 0 && pawn_canjump(us, from) ? shift_bb(SquareBB[from], pawn_push(us) * 2) &  ~occupied : 0;
			move |= attacks_from<PAWN>(from, us, occupied) & ~our;

			mi.destionations = move; mi.from = from;
			mvlist = mvlist << mi;
		}

		return mvlist->node;
	}

	template<GenerationType Captures, Color us>
	inline void generate(Represenation& rep, MoveList& mvlist)
	{



	}


}