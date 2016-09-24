#pragma once
#include "stdafx.h"


using std::string;
namespace Checkmate {

#define NO_CAPTURE_MAX_MOVES 50

	
	
	

	class Represenation
	{
	public: //Fields Exposed

		BoardStatePool boardStatePool;
		BoardState* state;
		Color sideToMove;
		Square enPassant;
		Square pieceList[COLOR_NB][PIECE_TYPE_NB][16];
		Bitboard colorbb[COLOR_NB]; //BLACK WHITE
		Bitboard typebb[PIECE_NB]; //WHITE_ROOK BLACK ROOK
		Bitboard pinnedbb = 0ULL;
		Bitboard Checkersbb = 0ULL;
		Piece board[SQUARE_NB];
		Key zorbist = 0;

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
	#pragma endregion

		void remove_piece(Square sq, Color c, PieceType pt);

		void put_piece(Square sq, Piece pc);
		void put_piece(Square sq, Color c, PieceType pt);

		int count(Piece p);
		int count(Color c);

		Piece opposite_color(Piece p);
		void remove_piece(Square sq, Piece pc);
		void move_piece(Square from, Square to, Color c, PieceType pt);

		void ClearSavedStates();

		bool is_board_ok();

		int can_castle(Color c, CastlingSide side);
		int can_castle(CastlingRight cr);
	private:

		bool is_ok_Board_Index_PieceList();

		#pragma region small_helper
				void init();

				void flipSideToMove();
				void makeNextState(Move mv, PieceType captrue, Key changes);

				void checkMove(Move m);

				
				
				int revoke_castling(Color c, CastlingRight cr);
				int revoke_castling(Color c);



				inline Bitboard GenerateChecker()
				{
					Color us = sideToMove; Color them = ~us; 
					return moves_for(KNIGHT, king_sqr(us), ~us, 0) & board_for(them, KNIGHT)
						| moves_for(BISHOP, king_sqr(us), ~us, colorbb[0] | colorbb[1]) & (board_for(them, BISHOP) | board_for(them, QUEEN))
						| moves_for(ROOK, king_sqr(us), ~us, colorbb[0] | colorbb[1]) & (board_for(them, ROOK) | board_for(them, QUEEN));

				}

				inline Bitboard GeneratePinned()
				{
					Color us = sideToMove; Color them = ~us;
					//#define Xray (moves_for(QUEEN, king_sqr(us), ~us, colorbb[0] | colorbb[1]) ^ colorbb[us])
					Bitboard Xray(moves_for(QUEEN, king_sqr(us), ~us, colorbb[0] | colorbb[1]) ^ colorbb[us]);
					return moves_for(BISHOP, king_sqr(us), ~us, Xray) & (board_for(them, BISHOP) | board_for(them, QUEEN))
						| moves_for(ROOK, king_sqr(us), ~us, Xray) & (board_for(them, ROOK) | board_for(them, QUEEN));
				}
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

			inline int Represenation::can_castle(Color c, CastlingSide side) {
				return castlingRights & (side << (2 * c));
			}

			inline int Represenation::revoke_castling(Color c, CastlingRight cr) {
				return castlingRights &= ~cr << (2*c);
			}

			inline int Represenation::revoke_castling(Color c) {
				return castlingRights &= ~((WHITE_OO | WHITE_OOO) << (2 * c));
			}


			inline Bitboard Represenation::board_for(Piece pc)
			{
				return typebb[pc];
			}
			inline Bitboard Represenation::board_for(PieceType pt)
			{
				return typebb[make_piece(WHITE, pt)] | typebb[make_piece(BLACK, pt)];
			}
			
			inline Bitboard Represenation::board_for(Color us,PieceType pt)
			{
				return typebb[(us << 3) | pt];
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


