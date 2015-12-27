#include "stdafx.h"

using namespace DataStructs;
namespace Checkmate
{

	SinglyLL<Move>* m_list;
	SinglyLL<Move>* m_nodeAt;

	SimpleMoveList::SimpleMoveList()
	{
		m_list = new SinglyLL<Move>(MOVE_NONE);
		m_nodeAt = nullptr;
	}

	SimpleMoveList::~SimpleMoveList()
	{
		m_list->clear();
		delete m_list;
	}

	void SimpleMoveList::Append(MoveInfo& mi)
	{
		Move m;
		Square to;
		PieceType capture;
		while (to = pop_lsb(mi.destionations), to != SQ_NONE)
		{
			capture = type_of(mi.rep.piece_on(to));
			switch (mi.movetype)
			{
			case CAPTURE:
				m = make<CAPTURE>(mi.from, to, mi.movingPt, capture);
				break;
			case NORMAL:
				m = make<NORMAL>(mi.from, to, mi.movingPt);
				break;
			case MoveType::CASTLING:
				m = make<CASTLING>(mi.from, to, mi.movingPt);
				break;
			case ENPASSANT:
				m = make<ENPASSANT>(mi.from, to, mi.movingPt);
				break;
			case PROMOTION:
				m = make_promotion_withCapture_move
					(mi.from, to, mi.movingPt, mi.promotion, capture);
				break;
			default:
				assert(false);
			}
			m_list = m_list->makeNext(m);
			m_nodeAt = m_list;
		}
	}

	bool SimpleMoveList::hasNext()
	{
		if(m_nodeAt == nullptr)
		{
			return false;
		}else
		{
			m_nodeAt->getIndex() > 0;
		}

	}

	Move SimpleMoveList::getNextOrDefault(Color c)
	{
		Move m;

		if(m_nodeAt == nullptr || m_nodeAt->getIndex() == 0)
		{
			m = MOVE_NONE;
			m_nodeAt = m_list;
		}else
		{
			m = m_nodeAt->value<Move>();
			m_nodeAt = m_nodeAt->node;
		}
		return m;
	}

	int SimpleMoveList::length()
	{
		return m_list->getIndex();
	}

	void SimpleMoveList::ResetGetNext()
	{
		m_nodeAt = m_list;
	}

	void SimpleMoveList::OnGeneratingNewMoves(Move m)
	{
		m_list->clear();
		m_nodeAt == nullptr;
	}

}