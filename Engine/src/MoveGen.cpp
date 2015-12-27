#include "stdafx.h"


namespace Checkmate {

	MoveGen::MoveGen(const Represenation& rep)
	: m_rep(*(new Represenation(rep))), m_moveGeninfo(*(new MoveGenInfo())){}

	void MoveGen::resetLookups()
	{
		
	}

	/// <summary>
	/// Collects attack information for all from_* lookups!!!
	/// </summary>
	/// <param name="us">Color us.</param>
	/// <param name="from">From square.</param>
	/// <param name="pt">The piecetype.</param>
	/// <param name="moves">Bitboard moves.</param>
	void MoveGen::collectAttackInfoFrom(Color us, Square from, PieceType pt, Bitboard moves)
	{
		Color them = ~us;
		Bitboard xray, pinned,blocker, attacks,check = 0ULL;
		Bitboard kingbb =  1ULL << m_rep.king_sqr(them);
		attacks = moves & enemybb;
		check = attacks  & kingbb;

		if (isSlidingPiece(pt))
		{
			xray = moves_for(pt, from, us, occupancy & ~attacks) & them;
			pinned = (kingbb & xray) == 0 ? 0ULL : moves_for(pt, from, us, kingbb) & attacks;
			blocker = (xray & enemybb) & ~pinned;
			xray &= enemybb;
			
			m_moveGeninfo.xray_from[us][from] = xray & check_filter(pt);
			m_moveGeninfo.pinned_from[us][from] = pinned & check_filter(pt);
			m_moveGeninfo.blocker_from[us][from] = blocker & check_filter(pt);
		}
		m_moveGeninfo.attackableSquares[us] |=  pt != PAWN ? moves : moves_for(PAWN,from,us,~0ULL);
		m_moveGeninfo.protectPieces[us] |= (moves & ourbb) & check_filter(pt);
		m_moveGeninfo.attacks_from[us][from] = attacks & check_filter(pt);
		m_moveGeninfo.checker_from[us][from] = check & check_filter(pt);
		
	}
	
	/// <summary>
	/// Applies lookups_from -&gt; lookups_to
	/// </summary>
	/// <param name="us">us</param>
	/// <param name="from">from square</param>
	/// <param name="pt">The piece type has to be supplied if append to list && (Set append to list true)</param>
	/// <param name="appendTolist">(Default false)if set to <c>true</c> [append tolist].</param>
	void MoveGen::collectAttackInfoTo(Color us, Square from, PieceType pt, bool appendTolist)
	{
		Square to = SQ_NONE; Color them = ~us;
		MoveInfo& mi = *(new MoveInfo(us, from, pt, NORMAL, m_rep));

		Bitboard Checks = m_moveGeninfo.checker_from[us][from];
		Bitboard Captures = m_moveGeninfo.attacks_from[us][from];
		Bitboard pinned = m_moveGeninfo.pinned_from[us][from];
		Bitboard attacks = m_moveGeninfo.attacks_from[us][from];
		Bitboard xray = m_moveGeninfo.xray_from[us][from];
		Bitboard blocker = m_moveGeninfo.blocker_from[us][from];

		while (to = pop_lsb(pinned), to != SQ_NONE)
		{
			m_moveGeninfo.pinned_to[them][to] |= SquareBB[from];
		}
		
		while (to = pop_lsb(Checks), to != SQ_NONE)
		{
			m_moveGeninfo.checker_to[them][to] |= SquareBB[from];
		}

		mi.movetype = CAPTURE;
		while (to = pop_lsb(attacks), to != SQ_NONE)
		{
			m_moveGeninfo.attacks_to[them][to] |= SquareBB[from];
		}
		mi.movetype = NORMAL;

		while (to = pop_lsb(xray), to != SQ_NONE)
		{
			m_moveGeninfo.xray_to[them][to] |= SquareBB[from];
		}

		while (to = pop_lsb(blocker), to != SQ_NONE)
		{
			m_moveGeninfo.blocker_to[them][to] |= SquareBB[from];
		}
		delete &mi;
	}

	MoveGenInfo& MoveGen::generate_all(Color us)
	{
		Color them = ~us; 
		m_moveGeninfo.OnGeneratingNewMoves(m_rep.state->lastMove);

		//What can my enemy do?
		switchColorUsTo(them); m_kingCheckfilter = ~0ULL;
		generate_attacks(them);
		
		//What can I do?
		switchColorUsTo(us);
		
		generate_normal(us);
		generate_attacks(us, true);
		generate_castleing(us);
		generate_promotion(us);
		generate_enpassant(us);
		m_moveGeninfo.isCheck = isCheck;

		return m_moveGeninfo;
	}

	MoveGenInfo& MoveGen::generate_normal(Color us)
	{
		MoveListBase* mvls = m_moveGeninfo.move_list;
		MoveInfo* mi; Square from = SQ_NONE;
		for (PieceType Pt = PAWN; Pt < PIECE_TYPE_NB; ++Pt)
		{
			mi = new MoveInfo(us, SQ_NONE, Pt, NORMAL,m_rep);
			for (int i = 0; i < 16 && (from = m_rep.pieceList[us][Pt][i], from != SQ_NONE); i++)
			{
				{
					mi->from = from;
					mi->destionations = (moves_for(Pt, from, us, occupancy) & ~occupancy) & check_filter(Pt);
					if (mi->destionations != 0)
					{

						mvls->Append(*mi);
					}
				}
			}
			delete mi;
		}
		m_moveGeninfo.move_list = mvls;
		return m_moveGeninfo;
	}

	MoveGenInfo& MoveGen::generate_castleing(Color us)
	{
		int castlerRights = m_rep.can_castle(us) >> (2*us);
		Square kingsqr = m_rep.king_sqr(us);
		Bitboard kingSidebb = SquareBB[relative_square(us, SQ_F1)] 
							| SquareBB[relative_square(us, SQ_G1)];

		Bitboard queenSidebb = SquareBB[relative_square(us, SQ_B1)]
							 | SquareBB[relative_square(us, SQ_C1)]
							 | SquareBB[relative_square(us, SQ_D1)];

		MoveListBase* mvls = m_moveGeninfo.move_list;
		MoveInfo* mi = new MoveInfo(us, SQ_NONE, KING, CASTLING, m_rep);

		mi->from = kingsqr;
		mi->destionations = 0ULL;

		if ((castlerRights & WHITE_OO) > 0 && (kingSidebb & occupancy) == 0 
			&& ((SquareBB[relative_square(us, SQ_G1)]) & check_filter(KING)) > 0)
		{
			mi->destionations |= SquareBB[relative_square(us, SQ_H1)];
		}

		if ((castlerRights & WHITE_OOO) > 0 && (queenSidebb & occupancy) == 0 
			&& ((SquareBB[relative_square(us, SQ_C1)]) & check_filter(KING)) > 0)
		{
			mi->destionations |= SquareBB[relative_square(us, SQ_A1)];
		}
		mvls->Append(*mi);
		delete mi;
		m_moveGeninfo.move_list = mvls;
		return m_moveGeninfo;
	}

	MoveGenInfo& MoveGen::generate_enpassant(Color us)
	{
		Square en = m_rep.enPassant; Color them = ~us;
		if (en != SQ_NONE)
		{
			Square from = SQ_NONE;
			Square to = en + pawn_push(us); 
			Bitboard moves = SquareBB[en];
			MoveListBase* mvls = m_moveGeninfo.move_list;
			moves = (shift_bb(moves, DELTA_W) | shift_bb(moves,DELTA_E)) & ourbb;
			
			while (from = pop_lsb(moves), from != SQ_NONE)
			{
				MoveInfo* mi = new MoveInfo(us, from, PAWN, MoveType::ENPASSANT,m_rep);
				mi->destionations = SquareBB[to];
				mvls->Append(*mi);
				delete mi;
			}
		}
		
		return m_moveGeninfo;
	}

	MoveGenInfo& MoveGen::generate_promotion(Color us)
	{
		Bitboard moves;
		Square from = SQ_NONE; 
		MoveInfo* mi;
		MoveListBase* mvls = m_moveGeninfo.move_list;
		for (int i = 0; i < 16 && (from = m_rep.pieceList[us][PAWN][i], from != SQ_NONE); i++)
		{
			moves = moves_for(PAWN, from, us, occupancy) & ~ourbb & check_filter(PAWN);
			if (relative_rank(us, from) == RANK_7 && (moves > 0))
			{
				mi = new MoveInfo(us, from, moves, PAWN, PROMOTION,m_rep);
				mi->promotion = QUEEN;
				mvls->Append(*mi); 
				mi->promotion = KNIGHT;
				mi->destionations = moves;
				mvls->Append(*mi);
				delete mi;
			}
		}
		m_moveGeninfo.move_list = mvls;
		return m_moveGeninfo;
	}
	
	void MoveGen::switchColorUsTo(Color us)
	{
		Color them = ~us;
		isCheck = m_moveGeninfo.checker_to[us][m_rep.king_sqr(us)] > 0;
		enemybb = m_rep.getColorbb(them);
		ourbb = m_rep.getColorbb(us);
		occupancy = enemybb | ourbb;
		makeCheckFilter(us);
		
	}

	void MoveGen::makeCheckFilter(Color us)
	{
		using namespace Bitboards;
		Square kingsqr = m_rep.king_sqr(us);
		Bitboard temp = 0ULL;
		Color them = ~us;
		m_checkfilter = isCheck ? 0 : ~0ULL;

		if (isCheck)
		{
			Square rookDelta [4][1] = { {DELTA_E}, {DELTA_W}, {DELTA_N}, {DELTA_S} };
			Square bishopDelta[4][1] = { { DELTA_NE },{ DELTA_NW },{ DELTA_SE },{ DELTA_SW} };
			Bitboard rookOrQueenbb = (m_rep.typebb[ROOK] | m_rep.typebb[QUEEN]) & enemybb;
			Bitboard bishopOrQueenbb = (m_rep.typebb[BISHOP] | m_rep.typebb[QUEEN]) & enemybb;;

			for (int i = 0; i < 4; i++)
			{
				temp = sliding_attack(kingsqr, rookDelta[i], 1, occupancy);
				m_checkfilter |= (temp & rookOrQueenbb) > 0 ? temp : 0ULL;
				
				temp = sliding_attack(kingsqr, bishopDelta[i], 1, occupancy);
				m_checkfilter |= (temp & bishopOrQueenbb) > 0 ? temp : 0ULL;
			}

			m_checkfilter |= m_moveGeninfo.attacks_to[us][kingsqr];
		}
		m_kingCheckfilter = ~m_moveGeninfo.attackableSquares[them];
	}

	void MoveGen::generate_attacks(Color us, bool appendToList)
	{
		Square from = SQ_NONE; Bitboard moves = 0ULL;
		for (PieceType pt = PAWN; pt < PIECE_TYPE_NB; ++pt)
		{
			for (int i = 0; i < 16 && (from = m_rep.pieceList[us][pt][i], from != SQ_NONE); i++)
			{
				if (pt != PAWN || (pt == PAWN && (relative_rank(us, from) != RANK_7)))
				{
					moves = moves_for(pt, from, us, occupancy);
					collectAttackInfoFrom(us, from, pt, moves);
					collectAttackInfoTo(us, from, pt, appendToList);
					if(appendToList)
					{
						appendAttacksToMoveList(from);
					}
				}
			}
		}
	}

	void MoveGen::appendAttacksToMoveList(Square from)
	{
		Color us = color_of(m_rep.piece_on(from));
		PieceType pt = type_of(m_rep.piece_on(from));
		MoveListBase* mvls = m_moveGeninfo.move_list;
		Bitboard Checks = m_moveGeninfo.checker_from[us][from];
		Bitboard Captures = m_moveGeninfo.attacks_from[us][from];

		if((Checks || Captures) && !m_moveGeninfo.is_pinned(us,from))
		{
			MoveInfo* mi = new MoveInfo(us, from, pt, NORMAL, m_rep);
			mi->destionations = Checks | Captures;
			mi->movetype = CAPTURE;
			mvls->Append(*mi);
			delete mi;
		}
	}

}