#include "stdafx.h"

using namespace DataStructs;
namespace Checkmate
{
	
	SimpleMoveList::SimpleMoveList()
	{
		listAt = 0;
		allLists[0] = new CustomList<Move>(8, MOVE_NONE);// rare moves high priority
		allLists[1] = new CustomList<Move>(50, MOVE_NONE);// captrues
		allLists[2] = new CustomList<Move>(220, MOVE_NONE);// normal
	}

	SimpleMoveList::~SimpleMoveList()
	{
		for (int i = 0; i < numLists; i++)
			delete allLists[i];
	}

	void SimpleMoveList::Append(MoveInfo& mi)
	{
		int listToAppend = 0;
		Move m;
		Square to;
		PieceType capture = NO_PIECE_TYPE;

		while (to = pop_lsb(mi.destionations), to != SQ_NONE)
		{
			capture = type_of(mi.rep.piece_on(to));
			switch (mi.movetype)
			{
			case CAPTURE:
				if (capture == NO_PIECE_TYPE)
					int a = 0;
				listToAppend = Captrues;
				m = make<CAPTURE>(mi.from, to, mi.movingPt, capture);
				break;
			case NORMAL:
				listToAppend = other;
				m = make<NORMAL>(mi.from, to, mi.movingPt);
				break;
			case MoveType::CASTLING:
				listToAppend = RareMoves;
				m = make<CASTLING>(mi.from, to, mi.movingPt);
				break;
			case ENPASSANT:
				listToAppend = RareMoves;
				m = make<ENPASSANT>(mi.from, to, mi.movingPt);
				break;
			case PROMOTION:
				listToAppend = RareMoves;
				m = make_promotion_withCapture_move
					(mi.from, to, mi.movingPt, mi.promotion, capture);
				break;
			default:
				assert(false);
			}

			allLists[listToAppend]->Append(m);
		}
	}

	bool SimpleMoveList::hasNext()
	{
		while (listAt < numLists)
		{
			if (!allLists[listAt]->hasNext())
				++listAt;
			else
				return true;
		}
		return false;
	}

	Move SimpleMoveList::getNextOrDefault()
	{
		Move m;

		if(hasNext())
		{
			return allLists[listAt]->GetNextOrDefault();
		}else
			m = MOVE_NONE;
		return m;
	}

	int SimpleMoveList::length()
	{
		int total = 0;
		for (int i = 0; i < numLists; ++i)
			total += allLists[i]->lastNode;
		return total;
	}

	void SimpleMoveList::Clear()
	{
		for (int i = 0; i < numLists; ++i)
			allLists[i]->Clear();
		listAt = 0;
	}

	void SimpleMoveList::ResetGetNext()
	{
		for (int i = 0; i < numLists; ++i)
			allLists[i]->ResetGetNext();
		listAt = 0;
	}

	void SimpleMoveList::OnGeneratingNewMoves(Move m)
	{
		Clear();
	}




}
