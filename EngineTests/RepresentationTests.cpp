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