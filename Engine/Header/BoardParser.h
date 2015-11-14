#pragma once
#include "stdafx.h"
namespace Checkmate {

	class BoardParser
	{
	public:
		BoardParser(::std::string fen);
		~BoardParser();

		void extractBoard();
		bool hasNextInstruction();
		void nextInstruction(PiecePlacement *&pTemp);
	private:
		
		void jumpFile(int jmp);
		void nextRank();
		PieceType toPiece(char piece);
		bool hasPiece();
		PiecePlacement* nextPiece();
		

	private: //Operation vars 
		DataStructs::DoubleLinkedList<PiecePlacement*> m_boardInstruction;
		::std::string m_FEN;
		int m_instructionAt = 0;
		Square m_squareAt;
		int m_charAt;
	};


	void createBoardParser(BoardParser *pointer, ::std::string newFen);

}


