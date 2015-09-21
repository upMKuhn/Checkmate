#pragma once
#include <string>
#include <iostream>
#include "../util/FEN.h"
#include "..\util\util.h"
#include "BitBoards.h"
#include "Header\DoubleLinkedList.h"

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
		
		Bitboard getPiecebb(Piece pc)
		{
			return this->piecebb[pc];
		}
		Bitboard getPiecebb(PieceType pt, Color c)
		{
			Piece pc = make_piece(c, pt);
			return this->piecebb[pc];
		}

		void setPiece(Square sq, Piece pc)
		{
			assert(Checkmate::is_ok(sq));
			Color c = color_of(pc);
			piecebb[pc] |= SquareBB[sq];
			colorbb[c] != SquareBB[sq];
			pieceLookup[sq] = pc;
		}
		void setPiece(Square sq,PieceType pt, Color c)
		{
			Piece pc = make_piece(c, pt);
			piecebb[pc] |= SquareBB[sq];
			colorbb[c] != SquareBB[sq];
			pieceLookup[sq] = pc;
		}


		bool canCastle(CastlingSide side)
		{
			return this->castelingRights[us][side];
		}
		bool canCastle(CastlingSide side, Color color)
		{
			return this->castelingRights[us][side];;
		}
		
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

