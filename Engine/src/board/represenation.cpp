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
		FEN_Parser fen(strFEN);
		PiecePlacement *instruct;
		fen.nextInstruction(instruct);
		while (instruct != NULL) 
		{
			setPiece(instruct->position, instruct->piece);
			fen.nextInstruction(instruct);
		} 
	

	}

#pragma region Properties


	Bitboard Represenation::getPiecebb(Piece pc)
	{
		return this->piecebb[pc];
	}
	Bitboard Represenation::getPiecebb(PieceType pt, Color c)
	{
		Piece pc = make_piece(c, pt);
		return this->piecebb[pc];
	}

	Bitboard Represenation::getColorbb(Piece pc)
	{
		return this->colorbb[pc];
	}
	Bitboard Represenation::getColorbb(PieceType pt, Color c)
	{
		Piece pc = make_piece(c, pt);
		return this->colorbb[pc];
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
		colorbb[c] |= SquareBB[sq];
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

		::std::fill_n(piecebb, PIECE_NB, NO_PIECE);
		::std::fill_n(pieceLookup, SQUARE_NB, NO_PIECE);
		::std::fill_n(colorbb, COLOR_NB, 0);
	}

	/// <summary>
	/// Is_ok check if all variables hold acceptable data
	/// in this class
	/// </summary>
	/// <returns>result</returns>
	bool Represenation::areAllBoardsOk()
	{
		bool test = true;
		Bitboard whiteBoard = 0, blackBoard = 0;
		
		whiteBoard = whiteBoard | piecebb[W_PAWN];
		whiteBoard = whiteBoard | piecebb[W_KNIGHT];
		whiteBoard = whiteBoard | piecebb[W_BISHOP];
		whiteBoard = whiteBoard | piecebb[W_ROOK];
		whiteBoard = whiteBoard | piecebb[W_QUEEN];
		whiteBoard = whiteBoard | piecebb[W_KING];

		blackBoard = blackBoard | piecebb[B_PAWN];
		blackBoard = blackBoard | piecebb[B_KNIGHT];
		blackBoard = blackBoard | piecebb[B_BISHOP];
		blackBoard = blackBoard | piecebb[B_ROOK];
		blackBoard = blackBoard | piecebb[B_QUEEN];
		blackBoard = blackBoard | piecebb[B_KING];
		
		test &= (whiteBoard & blackBoard) == 0;
		test &= whiteBoard == colorbb[WHITE];
		test &= blackBoard == colorbb[BLACK];

		Square sqr = SQ_A1;
		for each (Piece piece in pieceLookup)
		{
			if(piece != NO_PIECE)
			{
				Bitboard bb = getPiecebb(piece);
				test &= (bb & (1ULL << sqr)) > 0;
			}
			++sqr;
		}

		return test;
			   
	}


}