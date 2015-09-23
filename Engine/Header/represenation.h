#pragma once
#include "stdafx.h"

using std::string;
namespace Checkmate {

#define NO_CAPTURE_MAX_MOVES 50

	class Represenation
	{
	private:
		int movesSinceCapture;
		Color us,enemy;
		Square enPassant;
		Bitboard colorbb[COLOR_NB]; //BLACK WHITE
		Bitboard piecebb[PIECE_NB]; //WHITE_ROOK BLACK ROOK
		Piece pieceLookup[SQUARE_NB];
		bool  castelingRights[COLOR_NB][CASTLING_SIDE_NB];

	public:
		Represenation();
		Represenation(::std::string FEN);
		~Represenation();

		string boardToFEN();
		void fenToBoard(string strFEN);
		
		Bitboard getPiecebb(Piece pc);
		Bitboard getPiecebb(PieceType pt, Color c);

		Piece getPieceAt(Square position);

		void setPiece(Square sq, Piece pc);
		void setPiece(Square sq, PieceType pt, Color c);

		bool canCastle(CastlingSide side);
		bool canCastle(CastlingSide side, Color color);
		
	private:
		void init();
		bool is_ok();

		void flipSideToMove()
		{
			Color temp = enemy;
			enemy = us;
			us = temp;
		}
	};
}

