#pragma once
#include "stdafx.h"
namespace Checkmate {
	struct MoveInfo
	{
		MoveInfo(Bitboard destionations, Color Us, PieceType mving_pt, MoveType movetype, Square from)
			: destionations(destionations),
			Us(Us),
			mvingPt(mving_pt),
			movetype(movetype),
			from(from)
		{
		}

		Bitboard destionations = 0;
		PieceType mvingPt = NO_PIECE_TYPE;
		PieceType promotion = NO_PIECE_TYPE;
		MoveType movetype = NORMAL;
		Square from = SQ_NONE;
		Color Us = NO_COLOR;
	};

	struct MoveList {
		MoveList(Represenation& r)
		{
			rep = &r;
		}
		int index = 0;
		Move m = MOVE_NONE;
		MoveList* node;
		Represenation* rep;


		void clear()
		{
			m = MOVE_NONE;
		}

		MoveList* makeNext()
		{
			MoveList* nnode = new MoveList(*rep);
			nnode->node = &*this;
			nnode->index = index + 1;
			return nnode;
		}

		MoveList* append(Square from, Square to, MoveInfo& mi)
		{
			Color Us = mi.Us; 
			Piece caputre = rep->piece_on(from);
			PieceType caputrePt = (caputre != NO_PIECE) && !is_our(Us, caputre) ? type_of(caputre) : NO_PIECE_TYPE;
			
			assert(Us != NO_COLOR);
			MoveList* mvlist = makeNext();

			mvlist->m = mi.movetype == CASTLING ? make<CASTLING>(from, to) :
				mi.movetype == PROMOTION ? make<PROMOTION>(from, to, mi.promotion) :
				mi.movetype == ENPASSANT ?	make<ENPASSANT>(from, to) 
				: make<NORMAL>(from, to);
			type_of(mvlist->m);
			return mvlist;
		}

	};


	inline MoveList* extract_moves(MoveList* mvlist, MoveInfo& mi)
	{
		Square to = SQ_NONE; Square from = mi.from;
		Bitboard& bb = mi.destionations;
		Represenation*  rep = mvlist->rep;
		Color us = color_of(rep->piece_on(from));
		
		while (to = pop_lsb(bb), is_ok(to))
		{
			mvlist = &*(mvlist->append(from, to, mi));
		}
		return mvlist;
	}


	inline MoveList* operator<<(MoveList* mvlist, MoveInfo& mi)
	{
		mvlist = &*extract_moves(mvlist,mi);
		return mvlist;
	}

}