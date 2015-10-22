#include "stdafx.h"
#include "RepresentationTests.h"
using namespace Checkmate;

RepresentationTests::RepresentationTests()
{
	Checkmate::Bitboards::init();
}


RepresentationTests::~RepresentationTests()
{
}

TEST_F(RepresentationTests, MakeNormalMove)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move mv = make<NORMAL>(SQ_A2, SQ_C3, KNIGHT);
	board.makeMove(mv);
	EXPECT_EQ(board.boardToFEN(), "rnbqkbnr/pppppppp/8/8/8/4N3/PPPPPPPP/RNBQKB1R w KQkq - 0 1");
	EXPECT_EQ(W_KNIGHT, board.getPieceAt(SQ_E2));
}

TEST_F(RepresentationTests, MakeTwoMovesAndOnDoOneCheckFEN)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move mv = make<NORMAL>(SQ_G1, SQ_F3, KNIGHT);
	Move mv2 = make<NORMAL>(SQ_G7, SQ_G5, KNIGHT);
	board.makeMove(mv);
	board.makeMove(mv2);
	EXPECT_EQ(board.boardToFEN(), "rnbqkbnr/pppppppp/8/8/8/4N3/PPPPPPPP/RNBQKB1R w KQkq - 0 1");
	EXPECT_EQ(W_KNIGHT, board.getPieceAt(SQ_C3));
}

TEST_F(RepresentationTests, INITATE_BOARD_CHECK_PIECE_COUNT)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	int expectCount[PIECE_NB]; ::std::fill_n(expectCount, PIECE_NB, NO_PIECE);;
	expectCount[W_ROOK] = 2; expectCount[W_KNIGHT] = 2; expectCount[W_BISHOP] = 2;
	expectCount[W_QUEEN] = 1; expectCount[W_KING] = 1; expectCount[W_PAWN] = 8;
	expectCount[B_ROOK] = 2; expectCount[B_KNIGHT] = 2; expectCount[B_BISHOP] = 2;
	expectCount[B_QUEEN] = 1; expectCount[B_KING] = 1; expectCount[B_PAWN] = 8;

	EXPECT_TRUE(ARRAY_LENGHT(expectCount) == ARRAY_LENGHT(board.pieceCount));
	for (int i = W_PAWN; i < ARRAY_LENGHT(expectCount); i++)
	{
		EXPECT_EQ(expectCount[i], board.pieceCount[i]);
	}
}

TEST_F(RepresentationTests, BOARDS_ARE_IN_SYNC)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	EXPECT_TRUE(board.areAllBoardsOk());
}

TEST_F(RepresentationTests, BOARD_TO_FEN_ACCURATE)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq a3 0 1");
	EXPECT_EQ("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq a3 0 1", board.boardToFEN());
}