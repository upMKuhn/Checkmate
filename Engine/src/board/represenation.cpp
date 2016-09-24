#include "stdafx.h"

//Representation of the GameBoard

#ifdef CHECKS_ENABLED
#define DEBUG_CHECK_MOVE(x) checkMove(x)
#else
#define DEBUG_CHECK_MOVE(x)
#endif

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
		ClearSavedStates();
		delete state;
	}

	void Represenation::init()
	{
		state = new BoardState();
		sideToMove = WHITE;
		enPassant = SQUARE_NB;
		castlingRights = 0;
		moveClock = 0;
		fill_n(typebb, 16, NO_PIECE);
		fill_n(board, 64, NO_PIECE);
		fill_n(colorbb, 2, 0);
		fill_n(index, 64, 15);

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
		while (instruct != NULL && instruct->piece != NO_PIECE && instruct->position != SQ_NONE)
		{
			put_piece(instruct->position, instruct->piece);
			fen.nextInstruction(instruct);
		}

		sideToMove = fen.sideToMove;
		castlingRights = fen.castlingRights;
		moveClock = fen.FullMoveClock * 2 + fen.halfMoveClock;
		enPassant = fen.enPassant;
		ClearSavedStates();
		makeNextState(MOVE_NONE, NO_PIECE_TYPE);
	}

	string Represenation::boardToFEN()
	{
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
			fen += tolower(file_tochar(f));
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
		DEBUG_CHECK_MOVE(mv);
		Square from = from_sq(mv);
		Square to = to_sq(mv);
		PieceType movingPiece = moving_type(mv);
		PieceType capture = capture_type(mv);
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
			if (piece_on(relative_square(us, SQ_H8)) != make_piece(us, ROOK))
				revoke_castling(us, WHITE_OO);
			if (piece_on(relative_square(us, SQ_A8)) != make_piece(us, ROOK))
				revoke_castling(us, WHITE_OOO);
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
		CHECKS_ENABLED(assert(is_board_ok()));


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
		CHECKS_ENABLED(assert(is_board_ok()));

		BoardState* bs = state;
		state = oldState;
		Zorbist = state->zorbist;
		delete bs;
		
	}

	void Represenation::move_piece(Square from, Square to, Color c, PieceType pt)
	{
		Bitboard from_to_bb = SquareBB[from] | SquareBB[to];
		Piece p = make_piece(c, pt);

		typebb[p] ^= from_to_bb;
		typebb[ALL_PIECES] ^= from_to_bb;
		colorbb[c] ^= from_to_bb;
		pieceList[c][pt][index[from]] = to;
		index[to] = index[from];
		index[from] = 15;

		board[from] = NO_PIECE;
		board[to] = p;
	}

	void Represenation::put_piece(Square sq, Piece pc)
	{
		put_piece(sq, color_of(pc), type_of(pc));
	}

	void Represenation::put_piece(Square sq, Color c, PieceType pt)
	{
		assert(Checkmate::is_ok(sq));
		Piece pc = make_piece(c, pt);
		typebb[pc] |= SquareBB[sq];
		typebb[ALL_PIECES] |= SquareBB[sq];
		colorbb[c] |= SquareBB[sq];
		board[sq] = pc;
		pieceList[c][pt][pieceCount[c][pt]] = sq;
		index[sq] = pieceCount[c][pt];
		++pieceCount[c][pt];
		++pieceCount[c][ALL_PIECES];
	}


	inline void Represenation::remove_piece(Square sq, Color c, PieceType pt)
	{
		assert(Checkmate::is_ok(sq));
		if (board[sq] != NO_PIECE)
		{
			int listIndex = index[sq];
			int lastIndex = --pieceCount[c][pt];
			typebb[make_piece(c,pt)] ^= SquareBB[sq];
			typebb[ALL_PIECES] ^= SquareBB[sq];
			colorbb[c] ^= SquareBB[sq];
			board[sq] = NO_PIECE;
			index[sq] = 15;
			//Replace Piece to be removed with last
			pieceList[c][pt][listIndex] = pieceList[c][pt][lastIndex];
			pieceList[c][pt][lastIndex] = SQ_NONE;
			index[pieceList[c][pt][listIndex]] = listIndex;
			--pieceCount[c][ALL_PIECES];
		}
		CHECKS_ENABLED(assert(is_board_ok()));
	}


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
		Zorbist = state->zorbist;
	}

	void Represenation::ClearSavedStates()
	{
		BoardState* stateAt = state; BoardState* next = nullptr;
		while (state->stateIndex != 0)
		{
			next = state->next;
			delete state;
		}
	}

#pragma region DebugInfo

	/// <summary>
	/// Is_ok check if all variables hold acceptable data
	/// in this class
	/// </summary>
	/// <returns>result</returns>
	bool Represenation::is_board_ok()
	{
		bool test = true;
		Bitboard allPieces = 0;
		Bitboard Whitebb = colorbb[WHITE], Blackbb = colorbb[BLACK];

		for (Piece p = W_PAWN; p < PIECE_NB; ++p)
			allPieces |= typebb[p];

		test &= (Whitebb | Blackbb) == allPieces;
		test &= typebb[ALL_PIECES] == allPieces;

		Square sqr = SQ_A1;
		for each (Piece piece in board)
		{
			if (piece != NO_PIECE)
			{
				Color c = color_of(piece);
				PieceType pt = type_of(piece);
				Bitboard bb = board_for(piece);
				test &= (bb & SquareBB[sqr]) > 0;
				test &= pieceList[c][pt][index[sqr]] == sqr;
			}
			++sqr;
		}

		

		test = test && is_ok_Board_Index_PieceList();

		if (!test)
			std::cout << to_string() << endl;

		return test;
	}

	bool Represenation::is_ok_Board_Index_PieceList()
	{
		bool all_ok = true;
		const int INDEX_END = 256 , const DEFAULT = -1, const EMPTY_INDEX = PIECE_NB;
		int indexCheck[INDEX_END];
		fill_n(indexCheck, INDEX_END, DEFAULT);


		for (Square atSqr = SQ_A1; atSqr < SQUARE_NB; ++atSqr)
		{
			bool ok = true;
			int oldEntry = indexCheck[index[atSqr]];
			PieceType pt = type_of(board[atSqr]);
			Color c = color_of(board[atSqr]); 
			int pieceListEntry = pieceList[c][pt][index[atSqr]];
			int indexInIndexCheck = max(c, WHITE) * (256/2);
			indexInIndexCheck += (pt - 1) * 16;
			indexInIndexCheck += index[atSqr];


			//CONDITIONS
			if (pt == NO_PIECE_TYPE)
				ok &= index[atSqr] == 15;
			else
			{
				ok &= indexCheck[indexInIndexCheck] == DEFAULT; //No duplicate index references
				ok &= pieceList[c][pt][index[atSqr]] == atSqr;
				ok &= piece_on(atSqr) == make_piece(c, pt);
				indexCheck[indexInIndexCheck] = index[atSqr];
			}
 			all_ok &= ok;

		}
		return all_ok;
	}

#define AssertMove(test, move) {									\
		if (test == false)											\
		{															\
			std::cout << "Is board ok? " << is_board_ok() << endl;	\
			std::cout << to_string() << endl;						\
			std::cout << move_tostring(move) << endl;				\
			assert(false);											\
		}															\
	}																\


	void Represenation::checkMove(Move mv)
	{
		
		Square from = from_sq(mv);
		Square to = to_sq(mv);
		PieceType movingPiece = moving_type(mv);
		PieceType capture = capture_type(mv);
		MoveType mvtype = type_of(mv);

		Color us = sideToMove;
		Color them = ~us;

		AssertMove(piece_on(from) == make_piece(us, movingPiece), mv)

		
		

		if (mvtype == NORMAL || mvtype == PROMOTION)
		{
			//NORMAL && PROMOTION CHECK
			AssertMove((piece_on(to) == make_piece(them, capture)), mv);
		}
		
		if(mvtype == CASTLING)
		{
			AssertMove((piece_on(to) == make_piece(us, ROOK)), mv);
		}
		else if(mvtype == ENPASSANT)
		{
			AssertMove(piece_on(enPassant) == make_piece(them, PAWN), mv);
		}
		else if(mvtype == PROMOTION)
		{
			AssertMove(NO_PIECE_TYPE < promotion_type(mv)  && promotion_type(mv) < PIECE_TYPE_NB, mv);
		}
	}
	
	/// operator<<(Position) returns an ASCII representation of the position
	std::ostream& operator<<(std::ostream& os, Represenation& pos) {
		os << pos.to_string();
		return os;
	}

	string Represenation::to_string() {

		std::string str = "\n +---+---+---+---+---+---+---+---+\n";

		for (Rank r = RANK_8; r >= RANK_1; --r)
		{
			for (File f = FILE_A; f <= FILE_H; ++f)
			{
				str.append(" | ");
				str += piece_tochar(piece_on(make_square(f, r)));
			}
			
			str += " |\n +---+---+---+---+---+---+---+---+\n";


		}
		str += lastMove_tostring();
		str += "\nFEN: " + boardToFEN() + "\n"; 
		str += "Key: " + std::to_string(Zorbist::make_zorbist(this)) + "\n";
		return str;
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