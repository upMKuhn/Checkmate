#pragma once
#include "stdafx.h"
namespace Checkmate {
	struct MoveInfo
	{

		MoveInfo(Color Us, Square from, PieceType mving_pt, MoveType movetype,Represenation& refRep)
			: Us(Us),mvingPt(mving_pt),	movetype(movetype),	from(from), rep(Represenation(refRep)){}

		MoveInfo(Color Us, Square from, Bitboard destionations, PieceType mving_pt, MoveType movetype, Represenation& refRep)
			: destionations(destionations),	Us(Us),	mvingPt(mving_pt),movetype(movetype), from(from), rep(Represenation(refRep)) {}
		Represenation& rep;
		Bitboard destionations = 0ULL;
		PieceType mvingPt = NO_PIECE_TYPE;
		PieceType promotion = NO_PIECE_TYPE;
		MoveType movetype = NORMAL;
		Square from = SQ_NONE;
		Color Us = NO_COLOR;
	};

	struct MoveList {
		int index = 0;
		Move m = MOVE_NONE;
		Piece capture = NO_PIECE;
		MoveList* node;

		void clear()
		{
			m = MOVE_NONE;
		}

		MoveList* makeNext()
		{
			MoveList* nnode = new MoveList();
			nnode->node = &*this;
			nnode->index = index + 1;
			return nnode;
		}

		MoveList* append(Square from, Square to, MoveInfo& mi)
		{
			return append(from, to, mi, NO_PIECE);
		}
		MoveList* append(Square from, Square to, MoveInfo& mi, Piece capture)
		{
			Color Us = mi.Us;
			assert(Us != NO_COLOR);
			MoveList* mvlist = makeNext();
			mvlist->capture = capture;
			mvlist->m = mi.movetype == CASTLING ? make<CASTLING>(from, to) :
				mi.movetype == PROMOTION ? make<PROMOTION>(from, to, mi.promotion) :
				mi.movetype == ENPASSANT ? make<ENPASSANT>(from, to)
				: make<NORMAL>(from, to);
			return mvlist;
		}

	};

	inline MoveList* extract_moves(MoveList* mvlist, MoveInfo& mi)
	{
		Square to = SQ_NONE; Square from = mi.from;
		Bitboard& bb = mi.destionations;
		Color us = color_of(mi.rep.piece_on(from));
		
		while (to = pop_lsb(bb), is_ok(to))
		{
			mvlist = &*(mvlist->append(from, to, mi));
		}
		return mvlist;
	}


	inline MoveList* operator<<(MoveList*& mvlist, MoveInfo& mi)
	{
		mvlist = extract_moves(mvlist,mi);
		return mvlist;
	}

}