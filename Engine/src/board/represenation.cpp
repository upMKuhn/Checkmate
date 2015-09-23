#include "stdafx.h"

//Representation of the GameBoard

namespace Checkmate {

	Represenation::Represenation()
	{
		init();
	}

	Checkmate::Represenation::Represenation(::std::string FEN)
	{
		using namespace DataStructs;
		init();
		fenToBoard(FEN);

	}

	Represenation::~Represenation()
	{		
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

#pragma region TOOL Function


	Bitboard Represenation::getPiecebb(Piece pc)
	{
		return this->piecebb[pc];
	}
	Bitboard Represenation::getPiecebb(PieceType pt, Color c)
	{
		Piece pc = make_piece(c, pt);
		return this->piecebb[pc];
	}

	Piece Represenation::getPieceAt(Square position)
	{
		return pieceLookup[position];
	}

	void Represenation::setPiece(Square sq, Piece pc)
	{
		assert(Checkmate::is_ok(sq));
		setPiece(sq, type_of(pc), color_of(pc));
	}
	void Represenation::setPiece(Square sq, PieceType pt, Color c)
	{
		Piece pc = make_piece(c, pt);
		piecebb[pc] |= SquareBB[sq];
		colorbb[c] != SquareBB[sq];
		pieceLookup[sq] = pc;
	}

	bool Represenation::canCastle(CastlingSide side)
	{
		return this->castelingRights[us][side];
	}
	bool Represenation::canCastle(CastlingSide side, Color color)
	{
		return this->castelingRights[us][side];;
	}
#pragma endregion


	void Represenation::init()
	{
		movesSinceCapture = 0;
		us = WHITE; enemy = BLACK;
		enPassant = SQUARE_NB;
		castelingRights[WHITE][QUEEN_SIDE] = true;
		castelingRights[WHITE][KING_SIDE] = true;
		castelingRights[BLACK][KING_SIDE] = true;
		castelingRights[BLACK][QUEEN_SIDE] = true;
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