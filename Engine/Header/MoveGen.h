#pragma once
#include "stdafx.h"

namespace Checkmate {


	struct MoveInfo
	{
		MoveInfo(Bitboard destionations, PieceType mving_pt, MoveType movetype, Square from)
			: destionations(destionations),
			  mvingPt(mving_pt),
			  movetype(movetype),
			  from(from)
		{
		}

		Bitboard destionations = 0;
		PieceType mvingPt = NO_PIECE_TYPE;
		PieceType promotion = NO_PIECE_TYPE;
		MoveType movetype = NORMAL;
		Square from;
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
			nnode->index = index + 1 ;
			return nnode;
		}
	};



	inline MoveList* operator<<(MoveList* mvlist, MoveInfo& mi)
	{
		Bitboard& bb = mi.destionations;
		Square to = SQ_NONE; Square from = mi.from;
		
		Color us = color_of(mvlist->rep->piece_on(mi.from));
		
		Piece caputre = NO_PIECE;
		PieceType caputrePt = NO_PIECE_TYPE;
		do
		{
			to = pop_lsb(bb);
			if (is_ok(to))
			{
				
				caputre = mvlist->rep->piece_on(from);
				caputrePt = caputre != NO_PIECE && !is_our(us,caputre) ? type_of(caputre) : NO_PIECE_TYPE;

				mvlist->m = mi.movetype == CASTLING ? make<MoveType::CASTLING>(from, to, NO_PIECE_TYPE):
						mi.movetype == PROMOTION ? make<MoveType::PROMOTION>(mi.from, to, mi.promotion):
						mi.movetype == ENPASSANT && caputrePt != NO_PIECE_TYPE ?
									make<MoveType::PROMOTION>(mi.from, to, mi.promotion) :
								    make<MoveType::NORMAL>(mi.from, to, caputrePt);
				mvlist = mvlist->makeNext();
			}

		} while (is_ok(to));

		return mvlist;
	}

	enum GenerationType
	{
		Captures,EnPassant, Normal, Checks, Xray
	};

class MoveGen
{
public:
	
	Bitboard KnightAttacks[SQUARE_NB];
	Bitboard RookTemplate[SQUARE_NB];

public:

	MoveGen();
	~MoveGen();
	
	MoveList* generate_pawnMoves(Color us, MoveList* mvlist);

	template<GenerationType type, Color us>
	inline void generate(Represenation& rep, MoveList& mvlist);
	
	
	
	

private:
	void init();



};

}

