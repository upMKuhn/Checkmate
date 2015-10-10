#pragma once
#include "stdafx.h"

namespace Checkmate {

	/// <summary>
	/// Instructions to place piece on board
	/// </summary>

	class FEN_Parser
	{

	private:
		::std::string m_FEN;
		Color m_sideToMove;
		CastlingRight canCaste[NO_COLOR][CASTLING_SIDE_NB];
		Square enPassant;
		int MoveCounter;
		int halfMoveClock;
		int FullMoveClock;
		public:
			FEN_Parser(::std::string FEN);
			~FEN_Parser();

			void process();
			void nextInstruction(PiecePlacement *&pTemp);
		private:
			
			BoardParser* m_boardParser = NULL;

			void extractStates();
			
	};

	
	
}

