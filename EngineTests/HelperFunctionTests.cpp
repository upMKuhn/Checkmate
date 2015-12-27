#include "stdafx.h"
using namespace Checkmate;




TEST(MoveTypeTests, MakeEnpassantMoveCheckTypeIsCorrect)
{
	Move m = make<ENPASSANT>(SQ_A1, SQ_A2, PAWN);
	EXPECT_EQ(ENPASSANT, type_of(m));
}