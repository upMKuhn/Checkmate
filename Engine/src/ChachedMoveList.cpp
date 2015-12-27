#include "stdafx.h"

using namespace DataStructs;

namespace Checkmate {

	ChachedMoveList::ChachedMoveList()
	{
		init();
	}


	void ChachedMoveList::init()
	{
		//clt Casual List Types
		//slt Special List Types
		
		nullptr_t nt = nullptr;
		memset(m_casualList, 0, sizeof(m_casualList));
		memset(m_specialList, 0, sizeof(m_specialList));
		memset(m_CasualIndex, -1, sizeof(m_CasualIndex));
		memset(m_SpecialIndex, -1, sizeof(m_CasualIndex));
		memset(m_PieceIndex, NO_PIECE, sizeof(m_PieceIndex));
		memset(m_PieceCount, -1, sizeof(m_PieceCount));
	}


//Public functions 

	void ChachedMoveList::Append(MoveInfo& mi)
	{
		Move m;
		Square to = SQ_NONE;
		PieceType capture;


		while(to = pop_lsb(mi.destionations), to != SQ_NONE)
		{
			switch(mi.movetype)
			{
			case CAPTURE:
				capture = type_of(mi.rep.piece_on(to));
				m = make<CAPTURE>(mi.from, to, mi.movingPt, capture);

				if (capture == KING)
				{
					appendSpecialList(mi, CHECKS, mi.from, m);
				}else
				{
					appendCasualList(mi, CAPTURES, mi.from, m);
				}
				break;
			case NORMAL:
				m = make<NORMAL>(mi.from, to, mi.movingPt);
				appendCasualList(mi, NORMALS, mi.from, m);
				break;
			case MoveType::CASTLING:
				m = make<CASTLING>(mi.from, to, mi.movingPt);
				appendSpecialList(mi, CASTELING, mi.from, m);
				break;
			case ENPASSANT:
				m = make<ENPASSANT>(mi.from, to, mi.movingPt);
				appendSpecialList(mi, ENPASSANTS, mi.from, m);
				break;
			case PROMOTION:
				m = make_promotion_withCapture_move
					(mi.from, to, mi.movingPt, mi.promotion, type_of(mi.rep.piece_on(to)));
				appendSpecialList(mi, PROMOTIONS, mi.from, m);
				break;
			default:
				assert(false);
			}
			++m_lenght;
		}
	}

	bool ChachedMoveList::hasNext()
	{
		return !(specialListDone || casualListDone);
	}

	Move ChachedMoveList::getNextOrDefault(Color c )
	{
		Move m = MOVE_NONE;
		
		if(ListAt != nullptr && ListAt->getIndex() <= 0)
		{
			ListAt = nullptr;
		}

		if(!specialListDone && ListAt == nullptr)
		{
			ListAt = getNextNonEmptySpecialList(c);
		}
		if (ListAt == nullptr)
		{
			ListAt = getNextNonEmptyCasuallList(c);
		}

		m = ListAt == nullptr
			? MOVE_NONE
			: ListAt->value<Move>();
		ListAt = ListAt == nullptr
			? nullptr
			: ListAt->node;
		return m;
	}

	void ChachedMoveList::ResetGetNext()
	{
		ListAt = nullptr;
		specialIndexAt = 0;
		casualIndexAt = 0;
		specialTypeAt = CHECKS;
		casualListPieceTypeAt = PAWN;
		casualTypeAt = CAPTURES;
		specialListDone = false;
		casualListDone = false;

	}

	void ChachedMoveList::OnGeneratingNewMoves(Move m)
	{
		//TO BE IMPLEMENTED
	}

	int ChachedMoveList::length()
	{
		return m_lenght;
	}


//HELPER

	void ChachedMoveList::put_piece(Square s, Piece p)
	{
		Color c = color_of(p);
		PieceType pt = type_of(p);
		int index = ++m_PieceCount[c][pt];
		m_CasualIndex[s] = index;
		m_PieceIndex[s] = p;
		for (CasualList clt = CAPTURES; clt < CASUAL_LIST_NB; ++clt)
		{
			m_casualList[clt][c][pt][index] = new SinglyLL<Move>(MOVE_NONE);
		}
	}

	void ChachedMoveList::remove_piece(Square s)
	{
		Piece p = m_PieceIndex[s];
		if (p != NO_PIECE)
		{
			PieceType pt = type_of(p);
			Color c = color_of(p);
			int index = m_CasualIndex[s];
			int last = m_PieceCount[c][pt]--;
			m_CasualIndex[s] = -1;

			for (CasualList clt = CAPTURES; clt < CASUAL_LIST_NB; ++clt)
			{
				delete	(m_casualList[clt][c][pt][index]->clear());
				m_casualList[clt][c][pt][index] = m_casualList[clt][c][pt][last];
				m_casualList[clt][c][pt][last] = nullptr;
			}
		}
	}

	void ChachedMoveList::move_piece(Square from, Square to)
	{
		Piece p = m_PieceIndex[from];
		PieceType pt = type_of(p);
		Color c = color_of(p);
		int index = m_PieceCount[c][pt];
		remove_piece(to);

		for (CasualList clt = CAPTURES; clt < CASUAL_LIST_NB; ++clt)
		{
			m_casualList[clt][c][pt][index]->clear();
		}

		for (SpecialList slt = CHECKS; slt < CASUAL_LIST_NB; ++slt)
		{
			index = m_SpecialIndex[slt][from];
			if (index != -1)
			{
				m_casualList[slt][c][pt][index]->clear();
			}
		}
	}

	void ChachedMoveList::appendCasualList(MoveInfo & mi, CasualList cs, Square s, Move m)
	{
		int index = m_CasualIndex[s];
		int ListIndex = 0;
		SinglyLL<Move>* ls;
		if (index == -1)
		{
			put_piece(s, make_piece(mi.Us, mi.movingPt));
			index = m_CasualIndex[s];
			ListIndex = m_PieceCount[mi.Us][mi.movingPt];
			CHECKS_ENABLED(assert(m_casualList[cs][mi.Us][mi.movingPt][ListIndex] != nullptr));
			ls = m_casualList[cs][mi.Us][mi.movingPt][ListIndex];
			m_casualList[cs][mi.Us][mi.movingPt][ListIndex] = ls->makeNext(m);
		}
		else
		{
			ls = m_casualList[cs][mi.Us][mi.movingPt][index];
			m_casualList[cs][mi.Us][mi.movingPt][index] = ls->makeNext(m);
		}
	}

	void ChachedMoveList::appendSpecialList(MoveInfo & mi, SpecialList sl, Square s, Move m)
	{
		int index = m_SpecialIndex[sl][s];
		if (index = -1)
		{
			index = m_SpecialCount[sl]++;
			m_specialList[sl][mi.Us][index] = new SinglyLL<Move>(MOVE_NONE);
		}
		SinglyLL<Move>* ls = m_specialList[sl][mi.Us][index];
		m_specialList[sl][mi.Us][index] = ls->makeNext(m);
	}

	SinglyLL<Move>* ChachedMoveList::getNextNonEmptySpecialList(Color c)
	{
		bool listNotFound = true;
		SinglyLL<Move> *list;
		while (listNotFound)
		{
			if (specialIndexAt >= SPECIAL_LS_MAX)
			{
				specialIndexAt = 0;
				++specialTypeAt;
			}

			if (specialTypeAt >= SPECIAL_LIST_NB)
			{
				specialListDone = true;
				return nullptr;
			}

			list = m_specialList[specialTypeAt][c][specialIndexAt++];
			if (list != nullptr && list->getIndex() > 0)
			{
				return list;
				listNotFound = true;
			}
			else if (list == nullptr)
			{
				specialIndexAt = 0;
				++specialTypeAt;
			}
		}

	}
	SinglyLL<Move>* ChachedMoveList::getNextNonEmptyCasuallList(Color c)
	{
		bool listNotFound = true;
		SinglyLL<Move> *list;

		while (listNotFound)
		{
			if (casualIndexAt >= SPECIAL_LS_MAX)
			{
				casualIndexAt = 0;
				++casualListPieceTypeAt;
			}

			if (casualTypeAt >= CASUAL_LIST_NB)
			{
				casualListDone = true;
				return nullptr;
			}

			list = m_casualList[casualTypeAt][c][casualListPieceTypeAt][casualIndexAt++];

			if (list != nullptr && list->getIndex() > 0)
			{
				return list;
				listNotFound = true;
			}
			else if (casualListPieceTypeAt < KING)
			{
				casualIndexAt = 0;
				++casualListPieceTypeAt;
			}
			else if (casualListPieceTypeAt >= KING)
			{
				casualIndexAt = 0;
				casualListPieceTypeAt = PAWN;
				++casualTypeAt;
			}
		}
	}
}