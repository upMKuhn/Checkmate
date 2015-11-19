#pragma once
#include "stdafx.h"
namespace Checkmate {
	struct MoveInfo
	{

		MoveInfo(Color Us, Square from, PieceType mving_pt, MoveType movetype, Represenation& refRep)
			: Us(Us),movingPt(mving_pt),	movetype(movetype),	from(from), rep(refRep){}

		MoveInfo(Color Us, Square from, Bitboard destionations, PieceType mving_pt, MoveType movetype,  Represenation& refRep)
			: destionations(destionations),	Us(Us),	movingPt(mving_pt),movetype(movetype), from(from), rep(refRep) {}
		Represenation& rep;
		Bitboard destionations = 0ULL;
		PieceType movingPt = NO_PIECE_TYPE;
		PieceType promotion = NO_PIECE_TYPE;
		MoveType movetype = NORMAL;
		Square from = SQ_NONE;
		Color Us = NO_COLOR;
	};

	struct MoveList {
		int index = 0;
		Move m = MOVE_NONE;
		MoveList* node;
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
			
			switch(mi.movetype)
			{
			case NORMAL:
				mvlist->m = make<NORMAL>(
					from, to, mi.movingPt);
				break;
			case CAPTURE:
				mvlist->m = make<CAPTURE>(
					from, to, mi.movingPt, type_of(capture));
				break;
			case CASTLING:
				mvlist->m = make<CASTLING>(
					from, to, mi.movingPt);
				break;
			case ENPASSANT:
				mvlist->m = make<ENPASSANT>(
					from, to, mi.movingPt, PAWN);
				break;
			case PROMOTION:
				mvlist->m = make_promotion_withCapture_move(
					from, to, mi.movingPt, mi.promotion, type_of(capture));
				break;
			default:
				throw std::exception("Append: invalid move type supplied");
			}

			return mvlist;
		}
		
		void clear()
		{
			MoveList* ml = node;
			MoveList* temp;
			while(index > 0 && ml->index)
			{
				temp = ml->node;
				delete ml;
				ml = temp;
			}
			
			index = 0;
			m = MOVE_NONE;
		}
	};

	inline MoveList* extract_moves(MoveList* mvlist, MoveInfo& mi)
	{
		Square to = SQ_NONE; Square from = mi.from;
		Bitboard& bb = mi.destionations;
		Piece capture = mi.rep.piece_on(to);
		Color us = mi.Us;
		MoveType movetype = mi.movetype;
		while (to = pop_lsb(bb), is_ok(to))
		{
			capture = mi.rep.piece_on(to);
			mvlist =  movetype != CASTLING ? 
				&*(mvlist->append(from, to, mi, capture))
				: &*(mvlist->append(from, to, mi));
		}
		return mvlist;
	}
	inline MoveList* operator<<(MoveList*& mvlist, MoveInfo& mi)
	{
		mvlist = extract_moves(mvlist,mi);
		return mvlist;
	}

}