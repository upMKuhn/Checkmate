#include "stdafx.h"

//Representation of the GameBoard

namespace Checkmate {

	Represenation::Represenation()
	{
		init();
	}
	
	Represenation::Represenation(::std::string FEN)
	{
		using namespace DataStructs;
		init();
		fenToBoard(FEN);

	}

	void Represenation::init()
	{

		sideToMove = WHITE;
		enPassant = SQUARE_NB;
		castlingRights = 0;
		moveClock = 0;
		::std::fill_n(typebb, PIECE_NB, NO_PIECE);
		::std::fill_n(board, SQUARE_NB, NO_PIECE);
		::std::fill_n(colorbb, COLOR_NB, NO_PIECE);
		::std::fill_n(index, SQUARE_NB, NO_PIECE);

		for (Color c = WHITE; c < NO_COLOR; ++c)
		{
			for (PieceType pt = ALL_PIECES; pt < PIECE_TYPE_NB; ++pt)
			{
				for (int i = 0; i < 16; i++)
				{
					pieceList[c][pt][i] = SQ_NONE;
				}
				pieceCount[c][pt] = 0;
			}
		}
	}

	Represenation::~Represenation()
	{		
	}


	string Represenation::boardToFEN()
	{
		assert(areAllBoardsOk());
		int jumpFiles = 0; std::string fen = "";
		char charPieceTypes[] = {'!', 'p', 'n', 'b', 'r', 'q', 'k' };
		for (Rank r = RANK_8; r >= RANK_1; --r)
		{
			for (File f = FILE_A; f <= FILE_H;++f)
			{
				char charPiece = ' ';
				Piece pc = board[make_square(f, r)];
				if(pc != NO_PIECE)
				{
					//write Jump instruction
					if (jumpFiles != 0)
					{
						fen += '0' + jumpFiles;
						jumpFiles = 0;

					}

					charPiece = charPieceTypes[type_of(pc)];
					charPiece = color_of(pc) == WHITE ? toupper(charPiece): tolower(charPiece);
					fen += charPiece;
				}else
				{
					jumpFiles++;
				}
			}
			//write Jump instructions 
			if (jumpFiles != 0)
			{
				fen += '0' + jumpFiles;
				jumpFiles = 0;
			}
			if (r != RANK_1) {
				fen += '/';
			}
		}

		fen +=  (sideToMove == WHITE ? " w " : " b ");

		//Set Castle Rights
		if(can_castle(WHITE_OO))
		{
			fen += 'K';
		}
		if (can_castle(WHITE_OOO))
		{
			fen += 'Q';
		}

		if (can_castle(BLACK_OO))
		{
			fen += 'k';
		}
		if (can_castle(BLACK_OOO))
		{
			fen += 'q';
		}
		fen += ' ';

		if(enPassant != SQ_NONE)
		{
			Rank r = rank_of(enPassant);
			File f = file_of(enPassant);
			fen += file_tochar(f);
			fen += '0' + r+1;
			fen += ' ';
		}else
		{
			fen += "- ";
		}

		fen += std::to_string((moveClock));
		fen += ' ';
		fen += std::to_string(moveCounter);

		return fen;
	}

	void Represenation::fenToBoard(string strFEN)
	{
		FEN_Parser fen(strFEN);
		PiecePlacement *instruct;
		init();
		fen.nextInstruction(instruct);
		while (instruct != NULL) 
		{
			put_piece(instruct->position, instruct->piece);
			fen.nextInstruction(instruct);
		} 

		sideToMove = fen.sideToMove;
		castlingRights = fen.castlingRights;
		moveClock = fen.halfMoveClock;
		moveCounter = fen.FullMoveClock;
		enPassant = fen.enPassant;

	}

	bool Represenation::makeMove(Move mv)
	{
		//MakeMove
		assert(is_ok(mv));
		Square from = from_sq(mv);
		Square to = to_sq(mv);
		PieceType movingPiece = type_of(piece_on(from));
		PieceType capture = type_of(mv) == ENPASSANT? PAWN: type_of(piece_on(to));
		MoveType mvtype = type_of(mv);

		Color us = sideToMove;
		Color them = ~us;
		
		if (mvtype == CASTLING)
		{
			bool kingSide = from > to;
			Square rfrom = to;
			Square rto = relative_square(us, kingSide ? SQ_D1 : SQ_F1);
			Square to = relative_square(us, kingSide ? SQ_C1 : SQ_G1);
			move_piece(rfrom, rto, us, ROOK);
		}

		if (mvtype == ENPASSANT)
		{
			assert("ENPASSNT NOT IMPLEMENTED" == 0);
		}

		if (capture != NO_PIECE && mvtype != ENPASSANT)
		{
			remove_piece(to, ~us, capture);
		}

		if (mvtype != PROMOTION)
		{
			move_piece(from, to, us, movingPiece);
		}
		else {
			move_piece(from, to, us, movingPiece);
			remove_piece(to, make_piece(us,PAWN));
			put_piece(to, make_piece(us, promotion_type(mv)));
		}

		enPassant = mvtype == ENPASSANT ? SQ_NONE : enPassant;
		sideToMove = ~sideToMove;
		
		if(movingPiece == PAWN || capture != NO_PIECE_TYPE)
		{
			moveClock = 0;
		}else
		{
			moveClock++;
		}

		moveCounter = us == BLACK ? moveCounter + 1 : moveCounter;

		return false;
	}

	void Represenation::undoMove()
	{

	}

	
	/// <summary>
	/// Is_ok check if all variables hold acceptable data
	/// in this class
	/// </summary>
	/// <returns>result</returns>
	bool Represenation::areAllBoardsOk()
	{
		bool test = true;
		Bitboard allPieces = 0;
		Bitboard Whitebb = colorbb[WHITE], Blackbb = colorbb[BLACK];

		allPieces = allPieces | typebb[PAWN];
		allPieces = allPieces | typebb[KNIGHT];
		allPieces = allPieces | typebb[BISHOP];
		allPieces = allPieces | typebb[ROOK];
		allPieces = allPieces | typebb[QUEEN];
		allPieces = allPieces | typebb[KING];

		allPieces ^= Whitebb;
		allPieces ^= Blackbb;

		test &= allPieces == 0;

		Square sqr = SQ_A1;
		for each (Piece piece in board)
		{
			if(piece != NO_PIECE)
			{
				Bitboard bb = board_for(piece);
				test &= (bb & (1ULL << sqr)) > 0;
			}
			++sqr;
		}

		return test;
			   
	}


	/// operator<<(Position) returns an ASCII representation of the position
	std::ostream& operator<<(std::ostream& os, Represenation& pos) {
		os << pos.to_string();
		return os;
	}

	string Represenation::to_string() {

		string os = "\n +---+---+---+---+---+---+---+---+\n";

		for (Rank r = RANK_8; r >= RANK_1; --r)
		{
			for (File f = FILE_A; f <= FILE_H; ++f)
			{
				os += " | ";
				os += piece_tochar(piece_on(make_square(f, r)));
			}

			os += " |\n +---+---+---+---+---+---+---+---+\n";
		}

		return os;
	}
	
#pragma region Utillity

	/// <summary>
	/// A State is a compact representation of the state 
	/// positions and rights in a game position. This function 
	/// makes a snapshot and appends the state SLL
	/// </summary>
	void Represenation::makeNextState(Move mv, PieceType captrue)
	{
		BoardState* oldState = state;
		state = new BoardState();
		state->BLACK_BB = getColorbb(BLACK);
		state->WHITE_BB = getColorbb(WHITE);
		state->castlingRights = castlingRights;
		state->enPassant = enPassant;
		state->moveClock = moveClock;
		state->sideToMove = sideToMove;
		state->next = oldState;
		state->stateIndex = oldState->stateIndex + 1;
		oldState->lastMove = mv;
		oldState->captrue = captrue;
	}


#pragma endregion

}