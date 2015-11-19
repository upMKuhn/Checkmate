#pragma once
#include "stdafx.h"

using std::string;
namespace Checkmate {

#define NO_CAPTURE_MAX_MOVES 50

	struct BoardState
	{
		int castlingRights = 0;
		int moveClock = 0;
		int stateIndex = 0;

		Key zorbist = 0;

		Color sideToMove = NO_COLOR;
		Square enPassant = SQ_NONE;
		Move lastMove = MOVE_NONE;
		PieceType captrue = NO_PIECE_TYPE;
		Bitboard WHITE_BB = 0;
		Bitboard BLACK_BB = 0;
		BoardState* next;
	};
	
	

	class Represenation
	{
	public: //Fields Exposed

		BoardState* state;
		Color sideToMove;
		Square enPassant;
		Square pieceList[COLOR_NB][PIECE_TYPE_NB][16];
		Bitboard colorbb[COLOR_NB]; //BLACK WHITE
		Bitboard typebb[PIECE_NB]; //WHITE_ROOK BLACK ROOK
		Piece board[SQUARE_NB];

		int pieceCount[COLOR_NB][PIECE_TYPE_NB];
		int index[SQUARE_NB];
		int castlingRights;
		int moveClock = 0;
		

	public:
		Represenation();
		Represenation(::std::string FEN);
		~Represenation();

		string to_string();
		string boardToFEN();
		Square king_sqr(Color c);
		string lastMove_tostring();

		void undoMove();
		void fenToBoard(string strFEN);
		
		int can_castle(Color c);
		bool makeMove(Move mv);


	public:
		#pragma region Properties

		Bitboard board_for(Piece pc);
		Bitboard board_for(PieceType pt);
		Bitboard board_for(Color c, PieceType pt);

		Bitboard getColorbb(Color c);
		Piece piece_on(Square position);
		void remove_piece(Square sq, Color c, PieceType pt);

		void put_piece(Square sq, Piece pc);
		void put_piece(Square sq, Color c, PieceType pt);

		int count(Piece p);
		int count(Color c);

		Piece opposite_color(Piece p);
		void remove_piece(Square sq, Piece pc);

		void move_piece(Square from, Square to, Color c, PieceType pt);
		bool areAllBoardsOk();
	#pragma endregion

	private:
		#pragma region small_helper
				void init();

				void flipSideToMove();
				void makeNextState(Move mv, PieceType captrue);

				void checkMove(Move m);

				int can_castle(CastlingRight cr);
				int revoke_castling(Color c, CastlingRight cr);
				int revoke_castling(Color c);
		#pragma endregion
};
	
	#pragma region inline_helper

			inline void Represenation::flipSideToMove()
			{
				sideToMove = ~sideToMove;
			}

			inline Square Represenation::king_sqr(Color c)
			{
				return pieceList[c][KING][0];
			}

			inline int Represenation::can_castle(CastlingRight cr) {
				return castlingRights & cr;
			}

			inline int Represenation::can_castle(Color c) {
				return castlingRights & ((WHITE_OO | WHITE_OOO) << (2 * c));
			}

			inline int Represenation::revoke_castling(Color c, CastlingRight cr) {
				return castlingRights &= ~cr;
			}

			inline int Represenation::revoke_castling(Color c) {
				return castlingRights &= ~((WHITE_OO | WHITE_OOO) << (2 * c));
			}


			inline Bitboard Represenation::board_for(Piece pc)
			{
				return board_for(color_of(pc), type_of(pc));
			}
			inline Bitboard Represenation::board_for(PieceType pt)
			{
				return this->typebb[pt];
			}
			inline Bitboard Represenation::board_for(Color c,PieceType pt)
			{
				return this->typebb[pt] & colorbb[c];
			}

			inline Bitboard Represenation::getColorbb(Color c)
			{
				return this->colorbb[c];
			}


			inline Piece Represenation::piece_on(Square position)
			{
				return board[position];
			}

			inline Piece Represenation::opposite_color(Piece p)
			{
				return make_piece(~color_of(p), type_of(p));
			}

			inline void Represenation::remove_piece(Square sq, Piece pc)
			{
				remove_piece(sq, color_of(pc), type_of(pc));
			}
	

			inline int Represenation::count(Piece p)
			{
				return pieceCount[color_of(p)][type_of(p)];
			}

			inline int Represenation::count(Color c)
			{
				return pieceCount[c][ALL_PIECES];
			}

	#pragma endregion
}


