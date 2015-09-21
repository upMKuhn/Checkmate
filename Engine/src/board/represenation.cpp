#include "Represenation.h"

namespace Checkmate {

	Represenation::Represenation()
	{
		init();
	}

	Checkmate::Represenation::Represenation(::std::string FEN)
	{
		init();
		fenToBoard(FEN);
	}

	Represenation::~Represenation()
	{		
	}

	void Checkmate::Represenation::init()
	{
		int movesSinceCapture;
		us = WHITE; enemy = BLACK;
		enPassant = SQUARE_NB;
		castelingRights[WHITE][QUEEN_SIDE] = true;
		castelingRights[WHITE][KING_SIDE] = true;
		castelingRights[BLACK][KING_SIDE] = true;
		castelingRights[BLACK][QUEEN_SIDE] = true;
	}

	string Represenation::boardToFEN()
	{
		return "";
	}

	void Represenation::fenToBoard(string strFEN)
	{
		FEN_Phraser fen(strFEN);
		PiecePlacement *instruct;
		fen.nextInstruction(instruct);
		while (instruct != NULL) 
		{
			setPiece(instruct->position, instruct->piece);
			fen.nextInstruction(instruct);
		} 
	

	}

	/// <summary>
	/// Is_ok check if all variables hold acceptable data
	/// in this class
	/// </summary>
	/// <returns>result</returns>
	bool Represenation::is_ok()
	{
		bool test = false;
		Bitboard whiteBoard = 0, blackBoard = 0;
		
		test |= (whiteBoard && piecebb[W_PAWN]) == 0;
		whiteBoard = whiteBoard || piecebb[W_PAWN];
		test |= (whiteBoard && piecebb[W_KNIGHT]) == 0;
		whiteBoard = whiteBoard || piecebb[W_KNIGHT];
		test |= (whiteBoard && piecebb[W_BISHOP]) == 0;
		whiteBoard = whiteBoard || piecebb[W_BISHOP];
		test |= (whiteBoard && piecebb[W_ROOK]) == 0;
		whiteBoard = whiteBoard || piecebb[W_ROOK];
		test |= (whiteBoard && piecebb[W_QUEEN]) == 0;
		whiteBoard = whiteBoard || piecebb[W_QUEEN];
		test |= (whiteBoard && piecebb[W_KING]) == 0;
		whiteBoard = whiteBoard || piecebb[W_KING];

		test |= (whiteBoard && piecebb[B_PAWN]) == 0;
		blackBoard = whiteBoard || piecebb[B_PAWN];
		test |= (whiteBoard && piecebb[B_KNIGHT]) == 0;
		blackBoard = whiteBoard || piecebb[B_KNIGHT];
		test |= (whiteBoard && piecebb[B_BISHOP]) == 0;
		blackBoard = whiteBoard || piecebb[B_BISHOP];
		test |= (whiteBoard && piecebb[B_ROOK]) == 0;
		blackBoard = whiteBoard || piecebb[B_ROOK];
		test |= (whiteBoard && piecebb[B_QUEEN]) == 0;
		blackBoard = whiteBoard || piecebb[B_QUEEN];
		test |= (whiteBoard && piecebb[B_KING]) == 0;
		blackBoard = whiteBoard || piecebb[B_KING];
		
		test |= (whiteBoard && blackBoard) == 0;
		test |= whiteBoard == colorbb[WHITE];
		test |= blackBoard == colorbb[BLACK];
		return ~test;
			   
	}


}