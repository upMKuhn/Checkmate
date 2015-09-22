#pragma once
#include "stdafx.h"

namespace Checkmate {

	/// <summary>
	/// Instructions to place piece on board
	/// </summary>
	

	class FEN_Phraser
	{

		
		public:
			FEN_Phraser(::std::string FEN);
			~FEN_Phraser();

			void process();
			void nextInstruction(PiecePlacement *&pTemp);
		private:
			//Extract Board
			void extractBoard();
			void jumpFile(int jmp);
			void nextRank();
			PieceType toPiece(char piece);
			bool hasPiece();
			PiecePlacement nextPiece();
			bool hasNextInstruction();
				
			void updateFEN(::std::string FEN) { this->m_FEN = FEN; process(); }
		private:
			DataStructs::DoubleLinkedList<PiecePlacement> m_boardInstruction;
			int instructionAt = 0;
			::std::string m_FEN;
			Square m_squareAt;
			int m_charAt;

			//Properties
	public:
		void setFEN(::std::string FEN)
		{
			m_FEN = FEN;
			process();
		}
	};

	
	
}

