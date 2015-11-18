#include "stdafx.h"

//Representation of the GameBoard

namespace Checkmate {
	
	Represenation::Represenation()
	{
		init();
		fenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	}
	
	Represenation::Represenation(::std::string FEN)
	{
		using namespace DataStructs;
		init();
		fenToBoard(FEN);

	}

	Represenation::~Represenation()
	{		
	}

	void Represenation::init()
	{

		sideToMove = WHITE;
		enPassant = SQUARE_NB;
		castlingRights = 0;
		moveClock = 0;
		state = new BoardState();
		::std::fill_n(typebb, PIECE_NB, NO_PIECE);
		::std::fill_n(board, SQUARE_NB, NO_PIECE);
		::std::fill_n(colorbb, COLOR_NB, NO_PIECE);
		::std::fill_n(index, SQUARE_NB, PIECE_NB);

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

	void Represenation::fenToBoard(string strFEN)
	{
		FEN_Parser fen(strFEN);
		PiecePlacement *instruct;
		init();
		fen.nextInstruction(instruct);
		while (instruct != NULL)
		{
			if (instruct->piece != NO_PIECE && instruct->position != SQ_NONE) {
				put_piece(instruct->position, instruct->piece);
				fen.nextInstruction(instruct);
			}
		}

		sideToMove = fen.sideToMove;
		castlingRights = fen.castlingRights;
		moveClock = fen.FullMoveClock * 2 + fen.halfMoveClock;
		enPassant = fen.enPassant;
		makeNextState(MOVE_NONE, NO_PIECE_TYPE);
	}

	string Represenation::boardToFEN()
	{
		CHECKS_ENABLED(assert(areAllBoardsOk()));
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

		if(castlingRights == NULL)
		{
			fen += '-';
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

		fen += std::to_string((moveClock % 2));
		fen += ' ';
		fen += std::to_string(moveClock / 2);
		return fen;
	}

	bool Represenation::makeMove(Move mv)
	{
		//MakeMove
		CHECKS_ENABLED(assert(is_ok(mv)));
		Square from = from_sq(mv);
		Square to = to_sq(mv);
		PieceType movingPiece = type_of(piece_on(from));
		PieceType capture = type_of(mv) == ENPASSANT ? NO_PIECE_TYPE: type_of(piece_on(to));
		MoveType mvtype = type_of(mv);

		Color us = sideToMove;
		Color them = ~us;
		


		if (mvtype == CASTLING)
		{
			bool kingSide = from > to;
			Square rfrom = to;
			Square rto = relative_square(us, kingSide ? SQ_D1 : SQ_F1);
			to = relative_square(us, kingSide ? SQ_C1 : SQ_G1);
			move_piece(rfrom, rto, us, ROOK);
			capture = NO_PIECE_TYPE;
		}

		if (capture != NO_PIECE_TYPE)
		{
			remove_piece(to, ~us, capture);
		}

		if (mvtype == ENPASSANT)
		{
			remove_piece(enPassant, make_piece(them, PAWN));
		}

		if (mvtype == PROMOTION)
		{
			//Make Promotion
			remove_piece(from, make_piece(us, PAWN));
			put_piece(to, make_piece(us, promotion_type(mv)));
		}
		else{
			//Handle other move types
			move_piece(from, to, us, movingPiece);
		}

		if(movingPiece == ROOK)
		{
			CastlingRight cr = (CastlingRight)((relative_file(WHITE, from) == FILE_A ? WHITE_OOO
				: relative_file(WHITE, from) == FILE_H ? WHITE_OO : CASTLING_SIDE_NB) << (2 * us)) ;
			revoke_castling(us, cr);
		}else if(movingPiece == KING)
		{
			revoke_castling(us);
		}

		enPassant = relative_rank(us,from) == relative_rank(WHITE, SQ_A2)
					&& relative_rank(us, to) == relative_rank(WHITE, SQ_A4)
					? to : SQ_NONE;

		if(movingPiece == PAWN || capture != NO_PIECE_TYPE)
		{
			moveClock = 1;
		}else
		{
			moveClock++;
		}

		makeNextState(mv, capture);
		sideToMove = ~sideToMove;

		CHECKS_ENABLED(assert(state->zorbist != state->next->zorbist));

		return false;
	}

	void Represenation::undoMove()
	{
		CHECKS_ENABLED(assert(state->stateIndex > 0));
		BoardState* oldState = state->next;

		Move  mv = oldState->lastMove;
		Square to = from_sq(mv);
		Square from = to_sq(mv);
		PieceType movingPiece = type_of(piece_on(from));
		PieceType capture = oldState->captrue;
		MoveType mvtype = type_of(mv);

		Color us = oldState->sideToMove;
		Color them = ~us;

		if (mvtype == CASTLING)
		{
			bool kingSide = to > from;
			Square rto = relative_square(us, kingSide ? SQ_A1 : SQ_H1);
			Square rfrom = relative_square(us, kingSide ? SQ_D1 : SQ_F1);
			from = relative_square(us, kingSide ? SQ_C1 : SQ_G1);
			move_piece(rfrom, rto, us, ROOK);
			movingPiece = KING;
			capture = NO_PIECE_TYPE;
		}

		if (mvtype == ENPASSANT)
		{
			put_piece(oldState->enPassant, make_piece(them, PAWN));
		}

		if (mvtype == PROMOTION)
		{
			remove_piece(from, us, movingPiece);
			put_piece(to, us, PAWN);
		}
		else {
			//Handle Any other move
			move_piece(from, to, us, movingPiece);
		}

		if (capture != NO_PIECE && mvtype != ENPASSANT)
		{
			put_piece(from, them, capture);
		}

		castlingRights = oldState->castlingRights;
		enPassant = oldState->enPassant;
		sideToMove = oldState->sideToMove;

		moveClock = oldState->moveClock;

		CHECKS_ENABLED(assert(Zorbist::make_zorbist(this) == oldState->zorbist));

		BoardState* bs = state;
		state = oldState;
		delete bs;
		
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
		oldState->lastMove = mv;
		oldState->captrue = captrue;
		oldState->sideToMove = sideToMove;
		state = new BoardState();
		state->zorbist = Zorbist::make_zorbist(this);
		state->BLACK_BB = getColorbb(BLACK);
		state->WHITE_BB = getColorbb(WHITE);
		state->castlingRights = castlingRights;
		state->enPassant = enPassant;
		state->moveClock = moveClock;
		state->sideToMove = sideToMove;
		state->next = oldState;
		state->stateIndex = oldState->stateIndex + 1;
		

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
			if (piece != NO_PIECE)
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

	string square_toString(Square s)
	{
		string st = string();
		st += file_tochar(file_of(s));
		st += '0' + rank_of(s)+1;
		return st;
	}

	string move_tostring(Move m)
	{
		string st = string();
		st += "From: ";
		st += square_toString(from_sq(m));
		st += "\nTo: ";
		st += square_toString(to_sq(m));
		st += "\n";
		return st;
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
		os += lastMove_tostring();
		os += "\n\nKey: " + std::to_string(Zorbist::make_zorbist(this)) + "\n";
		return os;
	}

	string Represenation::lastMove_tostring()
	{
		if(state->stateIndex > 0)
		{
			return move_tostring(state->next->lastMove);
		}
		return string();
	}

#pragma endregion

}