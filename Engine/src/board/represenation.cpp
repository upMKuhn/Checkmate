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
		assert(areAllBoardsOk());
		int jumpFiles = 0; std::string fen = "";
		char charPieceTypes[] = {'!', 'p', 'n', 'b', 'r', 'q', 'k' };
		for (Rank r = RANK_8; r >= RANK_1; --r)
		{
			for (File f = FILE_A; f <= FILE_H;++f)
			{
				char charPiece = ' ';
				Piece pc = pieceLookup[make_square(f, r)];
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

		fen += std::to_string(halfMoveClock);
		fen += ' ';
		fen += std::to_string(FullMoveClock);
		return fen;
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

		sideToMove = fen.sideToMove;
		castlingRights = fen.castlingRights;
		halfMoveClock = fen.halfMoveClock;
		FullMoveClock = fen.FullMoveClock;
		enPassant = fen.enPassant;
		kingSquare = SQUARE_NB;

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

#pragma endregion

	void Represenation::init()
	{
		sideToMove = WHITE;
		enPassant = SQUARE_NB;
		castlingRights = 0;
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