#pragma once
#include "stdafx.h"

namespace Checkmate {

	class Engine_API SimpleMoveList :
		public MoveListBase
	{
	public:
		SimpleMoveList();
		~SimpleMoveList();

		virtual void Append(MoveInfo& mi) override;
		virtual Move getNextOrDefault() override;
		virtual void ResetGetNext() override;
		virtual void OnGeneratingNewMoves(Move m) override;
		virtual bool hasNext() override;
		virtual int  length() override;
		virtual void Clear() override;
	protected:
		const int RareMoves = 0, Captrues = 1, other = 2;

		int listAt = 0; const int numLists = 3;
		CustomList<Move>* allLists[3];
	};


}

