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

TEST_F(RepresentationTests, MakeLegalMove)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 0");
	Move mv = make<NORMAL>(SQ_B1, SQ_C3, KNIGHT);
	board.makeMove(mv);
	EXPECT_EQ(board.boardToFEN(), "rnbqkbnr/pppppppp/8/8/8/2N5/8/R1BQKBNR b KQkq - 0 1");
	EXPECT_EQ(W_KNIGHT, board.piece_on(SQ_C3));
}

TEST_F(RepresentationTests, Make_illegalMove)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 0");
	Move mv = make<NORMAL>(SQ_E1, SQ_F6, KNIGHT);
	EXPECT_FALSE(board.makeMove(mv));
	EXPECT_EQ(board.boardToFEN(), "rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 1");
}

TEST_F(RepresentationTests, MoveTwo_UndoOne_CheckFen)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move mv = make<NORMAL>(SQ_G1, SQ_F3, KNIGHT);
	Move mv2 = make<NORMAL>(SQ_G7, SQ_G5, KNIGHT);
	board.makeMove(mv);
	board.makeMove(mv2);
	EXPECT_EQ(W_KNIGHT, board.piece_on(SQ_G5));
	EXPECT_EQ(NO_PIECE, board.piece_on(SQ_G7));
	board.undoMove();
	EXPECT_EQ(board.boardToFEN(), "rnbqkbnr/pppppppp/8/8/8/4N3/PPPPPPPP/RNBQKB1R w KQkq - 0 1");
	EXPECT_EQ(W_KNIGHT, board.piece_on(SQ_G7));
}

TEST_F(RepresentationTests, INITATE_BOARD_CHECK_PIECE_COUNT)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	int expectCount[PIECE_NB]; ::std::fill_n(expectCount, PIECE_NB, NO_PIECE);;
	expectCount[W_ROOK] = 2; expectCount[W_KNIGHT] = 2; expectCount[W_BISHOP] = 2;
	expectCount[W_QUEEN] = 1; expectCount[W_KING] = 1; expectCount[W_PAWN] = 8;
	expectCount[B_ROOK] = 2; expectCount[B_KNIGHT] = 2; expectCount[B_BISHOP] = 2;
	expectCount[B_QUEEN] = 1; expectCount[B_KING] = 1; expectCount[B_PAWN] = 8;

	for (Piece i = W_PAWN; i < PIECE_NB; ++i)
	{
		if (!(i == 8 || i == 7))
		{
			EXPECT_EQ(expectCount[i], board.count(i));
		}
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