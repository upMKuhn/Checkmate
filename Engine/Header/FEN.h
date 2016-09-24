#pragma once
#include "stdafx.h"

namespace Checkmate {

	/// <summary>
	/// Instructions to place piece on board
	/// </summary>

	class Engine_API FEN_Parser
	{

	private:
		::std::string m_FEN;
		void init();
		public:
			FEN_Parser(::std::string FEN);
			~FEN_Parser();

			void nextInstruction(PiecePlacement *&pTemp);

			//Vars
			int castlingRights;
			Color sideToMove = NO_COLOR;
			Square enPassant = SQ_NONE;
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


