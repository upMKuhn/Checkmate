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

	MoveList* MoveGen::generate_all(Color us, MoveList* mvlist, Represenation& rep)
	{
		mvlist = generate_normal(us,mvlist,rep);
		mvlist = generate_captures(us, mvlist, rep);

		return mvlist;
	}


	MoveList * MoveGen::generate_normal(Color us, MoveList * mvlist, Represenation & rep)
	{
		Bitboard our = rep.getColorbb(us);
		Bitboard occupancy = our | rep.getColorbb(~us);

		for (PieceType Pt = PAWN; Pt < PIECE_TYPE_NB; ++Pt)
		{
			MoveInfo* mi = new MoveInfo(occupancy, us, Pt, NORMAL, SQ_NONE);
			for each(Square sqr in rep.pieceList[us][Pt])
			{
				if (sqr == SQ_NONE) { break; }
				mi->from = sqr;
				mi->destionations = moves_for(Pt, sqr, us, occupancy) & ~occupancy;
				mvlist = mvlist << *mi;
				
			}
			delete mi;
		}
		return mvlist;
	}

	MoveList * MoveGen::generate_captures(Color us, MoveList * mvlist, Represenation & rep)
	{
		Bitboard enemy = rep.getColorbb(~us);
		Bitboard occupancy = enemy | rep.getColorbb(us);

		for (PieceType Pt = PAWN; Pt < PIECE_TYPE_NB; ++Pt)
		{
			MoveInfo* mi = new MoveInfo(occupancy, us, Pt, NORMAL, SQ_NONE);
			for each(Square sqr in rep.pieceList[us][Pt])
			{
				if (sqr == SQ_NONE) { break; }
				mi->from = sqr;
				mi->destionations = moves_for(Pt, sqr, us, occupancy) & enemy;
				mvlist = mvlist << *mi;
			}
			delete mi;
		}
		return mvlist;
	}



}