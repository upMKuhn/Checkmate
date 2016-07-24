#pragma once
#include "stdafx.h"

namespace Checkmate {
	

class  Engine_API MoveGen
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
	
	MoveGenInfo* m_moveGeninfo = nullptr;
	Represenation& m_rep;
public:

	MoveGen(Represenation& rep);
	~MoveGen();

	MoveGenInfo& generate_all();
	MoveGenInfo& generate_all(Color us);

	MoveGenInfo& generate_normal(Color us);
	void generate_attacks(MoveInfo& mi);
	void generate_attacks(Color us, bool appendToList = false);
	void generate_promotion(MoveInfo& mi);
	MoveGenInfo& generate_castleing(Color us);
	MoveGenInfo& generate_enpassant(Color us);
	MoveGenInfo& getMoveGenInfo();

private:
	void resetLookups();
	void switchColorUsTo(Color us);
	void makeCheckFilter(Color us);
	void collectAttackInfoFrom(MoveInfo& mi);
	void collectAttackInfoTo(MoveInfo& mi);
	void appendAttacksToMoveList(MoveInfo& mi);
	Bitboard inline check_filter(PieceType pt)
	{
		return pt == KING ? m_kingCheckfilter : m_checkfilter;
	}
};

}

