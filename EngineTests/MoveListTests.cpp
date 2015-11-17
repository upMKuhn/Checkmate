#include "stdafx.h"
using namespace Checkmate;


TEST(MoveListTests,CheckMoveExtraction)
{
	Represenation& rep = *(new Represenation());
	MoveList* mls = new MoveList();
	MoveInfo* mi = new MoveInfo(WHITE,SQ_A1,(setSquare(SQ_H3) | setSquare(SQ_F3)), KNIGHT, NORMAL,rep);
	mls = extract_moves(mls, *mi);
	EXPECT_EQ(2, mls->index);
}

TEST(MoveListTests, CheckMoveExtractionThroughOperator)
{
	Represenation& rep = *(new Represenation());
	MoveList* mls = new MoveList();
	MoveInfo* mi = new MoveInfo(WHITE, SQ_A1,(setSquare(SQ_H3) | setSquare(SQ_F3)), KNIGHT, NORMAL, rep);
	mls << *mi;
	EXPECT_EQ(2, mls->index);
}