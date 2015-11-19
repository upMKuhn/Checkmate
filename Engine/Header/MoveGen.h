#pragma once
#include "stdafx.h"

namespace Checkmate {
	
	enum GenerationType
	{
		Captures,EnPassant, Normal, Checks, Xray
	};

	

class MoveGen
{
private:
	
	
	/// <summary>
	/// The checkfilter removes all illegal moves, 
	///	that don't make the piece pinned or attack a single checker
	/// </summary>
	Bitboard m_checkfilter = ~0ULL;
	Bitboard m_kingCheckfilter = ~0ULL;
	Bitboard ourbb, enemybb, occupancy = 0ULL;
	bool isCheck = false;
public:
	
	MoveGenInfo& m_moveGeninfo;
	Represenation& m_rep;
public:

	MoveGen(const Represenation& rep);

	MoveGenInfo& generate_all(Color us);
	MoveGenInfo& generate_normal(Color us);
	MoveGenInfo& generate_castleing(Color us);
	MoveGenInfo& generate_enpassant(Color us);
	MoveGenInfo& generate_promotion(Color us);
	void generate_attacks(Color us, bool appendToList = false);

private:
	void resetLookups();
	void switchColorUsTo(Color us);
	void makeCheckFilter(Color us);
	void collectAttackInfoFrom(Color us,Square from,PieceType pt ,Bitboard moves);
	void collectAttackInfoTo(Color us, Square from, PieceType pt = PAWN, bool appendTolist = false);

	Bitboard inline check_filter(PieceType pt)
	{
		return pt == KING ? m_kingCheckfilter : m_checkfilter;
	}
};

}

