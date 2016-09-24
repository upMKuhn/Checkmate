#pragma once
#include "stdafx.h"
namespace Checkmate
{
	class Engine_API MoveListBase abstract
	{
	public:
		virtual ~MoveListBase();

		virtual void Clear() abstract;
		virtual void Append(MoveInfo& mi) abstract;
		virtual Move getNextOrDefault() abstract;
		virtual void ResetGetNext() abstract;
		virtual void OnGeneratingNewMoves(Move m) abstract;
		virtual bool hasNext() abstract;
		virtual int  length() abstract;
	};

	MoveListBase* CreateNewMoveList();

}
