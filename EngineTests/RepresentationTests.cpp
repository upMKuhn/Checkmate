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
	Bitboards::init();
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	EXPECT_TRUE(board.areAllBoardsOk());
}