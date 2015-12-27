#pragma once
#include "stdafx.h"
namespace Checkmate
{
	class MoveListBase abstract
	{
	public:
		
		virtual void Append(MoveInfo& mi) abstract;
		virtual Move getNextOrDefault(Color c) abstract;
		virtual void ResetGetNext() abstract;
		virtual void OnGeneratingNewMoves(Move m) abstract;
		virtual bool hasNext() abstract;
		virtual int  length() abstract;
	};
}
