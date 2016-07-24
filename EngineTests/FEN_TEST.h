#pragma once
#include "stdafx.h"

using namespace Checkmate;

#define Engine_API __declspec(dllimport)

struct FEN_TEST_DATA
{
	FEN_TEST_DATA(::std::string str)
		: fen(str) {
		for (Square square = SQ_A1; square < SQUARE_NB; ++square)
		{
			board[square] = NO_PIECE;
		}
	}
	::std::string fen;
	Piece board[SQUARE_NB];

	void setPiece(Piece p,Square sq)
	{
		board[sq] = p;
	}
};

class FEN_TEST : public ::testing::Test
{
public:
	FEN_TEST();
	~FEN_TEST();
	std::string startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Bitboard parsePieceBoard(Piece board[]);
	void ASSERT_boardParser(FEN_TEST_DATA *testData, FEN_Parser *parser);
	

};

