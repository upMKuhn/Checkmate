#include "stdafx.h"


namespace Checkmate {

	MoveGen::MoveGen(Represenation& rep)
	: m_rep(rep)
	{
	}

	MoveGen::~MoveGen()
	{
		SAFE_DELETE(m_moveGeninfo);
	}

	/// <summary>
	/// Collects attack information for all from_* lookups!!!
	/// </summary>
	/// <param name="us">Color us.</param>
	/// <param name="from">From square.</param>
	/// <param name="pt">The piecetype.</param>
	/// <param name="moves">Bitboard moves.</param>
	void MoveGen::collectAttackInfoFrom(MoveInfo& mi)
	{
		PieceType pt = mi.movingPt;
		Square from = mi.from;
		Color them = ~mi.Us; Color us = mi.Us;
		Bitboard xray, pinned,blocker, attacks,check = 0ULL;
		Bitboard kingbb =  1ULL << m_rep.king_sqr(them);
		Bitboard moves = mi.destionations;
		attacks = moves & enemybb;
		check = attacks  & kingbb;

		if (isSlidingPiece(pt))
		{
			xray = moves_for(pt, from, us, occupancy & ~attacks) & them;
			pinned = (kingbb & xray) == 0 ? 0ULL : moves_for(pt, from, us, kingbb) & attacks;
			blocker = (xray & enemybb) & ~pinned;
			xray &= enemybb;
			
			getMoveGenInfo().xray_from[us][from] = xray & check_filter(pt);
			getMoveGenInfo().pinned_from[us][from] = pinned & check_filter(pt);
			getMoveGenInfo().blocker_from[us][from] = blocker & check_filter(pt);
		}
		getMoveGenInfo().attackableSquares[us][from] =  pt != PAWN 
			? moves : moves_for(PAWN,from,us,~0ULL);
		getMoveGenInfo().attackableSquares[us][SQUARE_NB] |= getMoveGenInfo().attackableSquares[us][from];
		getMoveGenInfo().protectPieces[us] |= (moves & ourbb) & check_filter(pt);
		getMoveGenInfo().attacks_from[us][from] = attacks & check_filter(pt);
		getMoveGenInfo().checker_from[us][from] = check & check_filter(pt);
		
	}
	
	/// <summary>
	/// Applies lookups_from -&gt; lookups_to
	/// </summary>
	/// <param name="us">us</param>
	/// <param name="from">from square</param>
	/// <param name="pt">The piece type has to be supplied if append to list && (Set append to list true)</param>
	/// <param name="appendTolist">(Default false)if set to <c>true</c> [append tolist].</param>
	void MoveGen::collectAttackInfoTo(MoveInfo& mi)
	{
		Square from = mi.from;
		Square to = SQ_NONE; Color us = mi.Us; Color them = ~us;

		Bitboard Checks = getMoveGenInfo().checker_from[us][from];
		Bitboard Captures = getMoveGenInfo().attacks_from[us][from];
		Bitboard pinned = getMoveGenInfo().pinned_from[us][from];
		Bitboard attacks = getMoveGenInfo().attacks_from[us][from];
		Bitboard xray = getMoveGenInfo().xray_from[us][from];
		Bitboard blocker = getMoveGenInfo().blocker_from[us][from];

		while (to = pop_lsb(pinned), to != SQ_NONE)
		{
			getMoveGenInfo().pinned_to[them][to] |= SquareBB[from];
		}
		
		while (to = pop_lsb(Checks), to != SQ_NONE)
		{
			getMoveGenInfo().checker_to[them][to] |= SquareBB[from];
		}

		while (to = pop_lsb(attacks), to != SQ_NONE)
		{
			getMoveGenInfo().attacks_to[them][to] |= SquareBB[from];
		}

		while (to = pop_lsb(xray), to != SQ_NONE)
		{
			getMoveGenInfo().xray_to[them][to] |= SquareBB[from];
		}

		while (to = pop_lsb(blocker), to != SQ_NONE)
		{
			getMoveGenInfo().blocker_to[them][to] |= SquareBB[from];
		}
	}

	MoveGenInfo& MoveGen::generate_all()
	{
		return generate_all(m_rep.sideToMove);
	}

	MoveGenInfo& MoveGen::generate_all(Color us)
	{
		Color them = ~us;
		getMoveGenInfo().OnGeneratingNewMoves(MOVE_NONE);
		//What can my enemy do?
		switchColorUsTo(them); m_kingCheckfilter = ~0ULL;
		generate_attacks(them);
		
		//What can I do?
		switchColorUsTo(us);
		
		generate_normal(us);
		generate_castleing(us);
		generate_enpassant(us);
		getMoveGenInfo().isCheck = isCheck;
		getMoveGenInfo().move_list->ResetGetNext();
		return getMoveGenInfo();
	}

	MoveGenInfo& MoveGen::generate_normal(Color us)
	{
		MoveListBase* mvls = getMoveGenInfo().move_list;
		Square from = SQ_NONE;
		MoveInfo mi(us, SQ_NONE, PAWN, NORMAL, m_rep);
		for (PieceType Pt = PAWN; Pt < PIECE_TYPE_NB; ++Pt)
		{
			for (int i = 0; i < 16 && (from = m_rep.pieceList[us][Pt][i], from != SQ_NONE); i++)
			{
				mi.from = from;
				mi.movingPt = Pt;
				mi.destionations = (moves_for(Pt, from, us, occupancy) & ~occupancy) & check_filter(Pt);
				collectAttackInfoFrom(mi);
				collectAttackInfoTo(mi);
				generate_attacks(mi);
				if (mi.destionations != 0)
				{
					mi.movetype = NORMAL;
					mvls->Append(mi);
					generate_promotion(mi);
				}
			}
		}
		getMoveGenInfo().move_list = mvls;
		return getMoveGenInfo();
	}

	void MoveGen::generate_attacks(MoveInfo& mi)
	{
		Bitboard moves = 0ULL;

		if (mi.movingPt != PAWN || (mi.movingPt == PAWN && (relative_rank(mi.Us, mi.from) != RANK_7)))
		{
			moves = (moves_for(mi.movingPt, mi.from, mi.Us, occupancy) & enemybb) & check_filter(mi.movingPt);
			if (moves > 0)
			{
				
				MoveListBase* mvls = getMoveGenInfo().move_list;

				if (!getMoveGenInfo().is_pinned(mi.Us, mi.from))
				{
					mi.movetype = CAPTURE;
					mi.destionations = moves;
					collectAttackInfoFrom(mi);
					collectAttackInfoTo(mi);
					mvls->Append(mi);
				}

			}
		}
	}

	void MoveGen::generate_promotion(MoveInfo& mi)
	{
		Bitboard moves = 0; 
		mi.movetype = PROMOTION;
		if (relative_rank(mi.Us, mi.from) == RANK_7 && (moves > 0))
		{
			moves = moves_for(PAWN, mi.from, mi.Us, occupancy) & ~ourbb & check_filter(PAWN);
			if (moves > 0)
			{
				mi.promotion = QUEEN;
				getMoveGenInfo().move_list->Append(mi);
				mi.promotion = KNIGHT;
				mi.destionations = moves;
				getMoveGenInfo().move_list->Append(mi);
			}
		}
	}
	
	void MoveGen::generate_attacks(Color us, bool appendToList)
	{
		Square from = SQ_NONE; Bitboard moves = 0ULL;
		MoveInfo mi(us, SQ_NONE, NO_PIECE_TYPE, CAPTURE, m_rep);
		for (PieceType pt = PAWN; pt < PIECE_TYPE_NB; ++pt)
		{
			for (int i = 0; i < 16 && (from = m_rep.pieceList[us][pt][i], from != SQ_NONE); i++)
			{
				if (pt != PAWN || (pt == PAWN && (relative_rank(us, from) != RANK_7)))
				{
					mi.from = from;
					mi.movingPt = pt;
					mi.destionations = moves_for(pt, from, us, occupancy) & check_filter(pt);
					collectAttackInfoFrom(mi);
					collectAttackInfoTo(mi);
					mi.destionations &= enemybb;
					if (moves > 0)
					{
						mi.from = from;
						mi.movingPt = pt;
						mi.destionations = moves;
						mi.movetype = CAPTURE;
						if (appendToList)
						{
							appendAttacksToMoveList(mi);
						}
					}
				}
			}
		}
		collectAttackInfoTo(mi);
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

		MoveListBase* mvls = getMoveGenInfo().move_list;
		MoveInfo mi(us, SQ_NONE, KING, CASTLING, m_rep);

		mi.from = kingsqr;
		mi.destionations = 0ULL;

		if (kingsqr == relative_square(us, SQ_E1))
		{
			if (m_rep.can_castle(us, KING_SIDE) > 0 && (kingSidebb & occupancy) == 0
				&& ((SquareBB[relative_square(us, SQ_G1)]) & check_filter(KING)) > 0)
			{
				if(m_rep.piece_on(relative_square(us,SQ_H8)) == make_piece(us, ROOK))
					mi.destionations |= SquareBB[relative_square(us, SQ_H1)];
			}

			if (m_rep.can_castle(us, QUEEN_SIDE) > 0 && (queenSidebb & occupancy) == 0
				&& ((SquareBB[relative_square(us, SQ_C1)]) & check_filter(KING)) > 0)
			{
				if (m_rep.piece_on(relative_square(us,SQ_A8)) == make_piece(us, ROOK))
					mi.destionations |= SquareBB[relative_square(us, SQ_A1)];
			}
		}

		mvls->Append(mi);
		getMoveGenInfo().move_list = mvls;
		return getMoveGenInfo();
	}

	MoveGenInfo& MoveGen::generate_enpassant(Color us)
	{
		Square en = m_rep.enPassant; Color them = ~us;
		Piece enPiece = m_rep.piece_on(en);
		if (en != SQ_NONE && color_of(enPiece) == them && type_of(enPiece) == PAWN)
		{
			Square from = SQ_NONE;
			Square to = en + pawn_push(us); 
			Bitboard moves = SquareBB[en];
			MoveListBase* mvls = getMoveGenInfo().move_list;
			moves = (shift_bb(moves, DELTA_W) | shift_bb(moves,DELTA_E)) & m_rep.board_for(make_piece(us,PAWN));
			
			while (from = pop_lsb(moves), from != SQ_NONE)
			{
				MoveInfo* mi = new MoveInfo(us, from, PAWN, MoveType::ENPASSANT,m_rep);
				mi->destionations = SquareBB[to];
				mvls->Append(*mi);
				delete mi;
			}
		}
		
		return getMoveGenInfo();
	}

	
	void MoveGen::switchColorUsTo(Color us)
	{
		Color them = ~us;
		isCheck = getMoveGenInfo().checker_to[us][m_rep.king_sqr(us)] > 0;
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
			Bitboard rookOrQueenbb = (m_rep.board_for(ROOK) | m_rep.board_for(QUEEN)) & enemybb;
			Bitboard bishopOrQueenbb = (m_rep.board_for(BISHOP) | m_rep.board_for(QUEEN)) & enemybb;;

			for (int i = 0; i < 4; i++)
			{
				temp = sliding_attack(kingsqr, rookDelta[i], 1, occupancy);
				m_checkfilter |= (temp & rookOrQueenbb) > 0 ? temp : 0ULL;
				
				temp = sliding_attack(kingsqr, bishopDelta[i], 1, occupancy);
				m_checkfilter |= (temp & bishopOrQueenbb) > 0 ? temp : 0ULL;
			}

			m_checkfilter |= getMoveGenInfo().attacks_to[us][kingsqr] & ~ourbb;
		}
		m_kingCheckfilter = ~getMoveGenInfo().attackableSquares[them][SQUARE_NB] & ~ourbb;
	}


	void MoveGen::appendAttacksToMoveList(MoveInfo& mi)
	{
		Square from = mi.from;
		Color us = color_of(m_rep.piece_on(from));
		PieceType pt = type_of(m_rep.piece_on(from));
		MoveListBase* mvls = getMoveGenInfo().move_list;
		Bitboard Checks = getMoveGenInfo().checker_from[us][from];
		Bitboard Captures = getMoveGenInfo().attacks_from[us][from];

		if((Checks || Captures) && !getMoveGenInfo().is_pinned(us,from))
		{
			mi.movingPt = pt;
			mi.destionations = Checks | Captures;
			mi.movetype = CAPTURE;
			mvls->Append(*&mi);
		}
	}

	MoveGenInfo& MoveGen::getMoveGenInfo()
	{
		if (m_moveGeninfo == nullptr)
			m_moveGeninfo = new MoveGenInfo();
		return *m_moveGeninfo;
	}


}