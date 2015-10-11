#pragma once
#include "stdafx.h"

using std::string;
namespace Checkmate {

#define NO_CAPTURE_MAX_MOVES 50

	class Represenation
	{
	public:

		Color sideToMove;
		Square enPassant;
		Square kingSquare;
		Square board[SQUARE_NB];
		Bitboard colorbb[COLOR_NB]; //BLACK WHITE
		Bitboard piecebb[PIECE_NB]; //WHITE_ROOK BLACK ROOK
		Bitboard castelingPath[CASTLING_RIGHT_NB];
		Piece pieceLookup[SQUARE_NB];
		int pieceCount [COLOR_NB][PIECE_TYPE_NB];
		int castlingRights;
		int halfMoveClock;
		int FullMoveClock;

	public:
		Represenation();
		Represenation(::std::string FEN);
		~Represenation();

		string boardToFEN();
		void fenToBoard(string strFEN);

		int can_castle(Color c);
		int can_castle(CastlingRight cr);

#pragma region Properties
		
		Bitboard getPiecebb(Piece pc);
		Bitboard getPiecebb(PieceType pt, Color c);

		Bitboard getColorbb(Piece pt);
		Bitboard getColorbb(PieceType pt, Color c);

		Piece getPieceAt(Square position);

		void setPiece(Square sq, Piece pc);
		void setPiece(Square sq, PieceType pt, Color c);

		bool areAllBoardsOk();
#pragma endregion

	private:
		void init();
		void flipSideToMove()
		{
			sideToMove = ~sideToMove;
		}
	};
	
	inline int Represenation::can_castle(CastlingRight cr) {
		return castlingRights & cr;
	}

	inline int Represenation::can_castle(Color c) {
		return castlingRights & ((WHITE_OO | WHITE_OOO) << (2 * c));
	}
}


