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
		
		public:
			FEN_Parser(::std::string FEN);
			~FEN_Parser();

			
			void nextInstruction(PiecePlacement *&pTemp);

			Color m_sideToMove;
			CastlingRight m_canCaste[NO_COLOR][CASTLING_SIDE_NB];
			Square enPassant;
			int MoveCounter;
			int halfMoveClock;
			int FullMoveClock;
		private:
			
			BoardParser* m_boardParser = NULL;
			int getEndOfRepresenation();
			void jumpSpaceChars(int & index);
			void extractStates();
			void extractSideToMove(int & index);
			void extractCastelingRights(int& index);
			void extractEnPassant(int & index);
			void extractMoveCounter(int& index);
			void process();
			
	};

	
	
}

