#pragma once
#include "stdafx.h"

namespace Checkmate {
	class SimpleMoveList :
		public MoveListBase
	{
	public:
		SimpleMoveList();
		~SimpleMoveList();

		virtual void Append(MoveInfo& mi) override;
		virtual Move getNextOrDefault(Color c) override;
		virtual void ResetGetNext() override;
		virtual void OnGeneratingNewMoves(Move m) override;
		virtual bool hasNext() override;
		virtual int  length() override;
	};
}

