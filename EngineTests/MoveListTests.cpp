#include "stdafx.h"
using namespace Checkmate;


TEST(MoveListTests,CheckMoveExtraction)
{
	Represenation* rep = new Represenation();
	MoveList* mls = new MoveList(*rep);
	MoveInfo* mi = new MoveInfo((setSquare(SQ_H3) | setSquare(SQ_F3)), WHITE, KNIGHT, NORMAL, SQ_G1);
	mls = extract_moves(mls, *mi);
	EXPECT_EQ(2, mls->index);
}

TEST(MoveListTests, CheckMoveExtractionThroughOperator)
{
	Represenation* rep = new Represenation();
	MoveList* mls = new MoveList(*rep);
	MoveInfo* mi = new MoveInfo((setSquare(SQ_H3) | setSquare(SQ_F3)), WHITE, KNIGHT, NORMAL, SQ_G1);
	mls = mls << *mi;
	EXPECT_EQ(2, mls->index);
}