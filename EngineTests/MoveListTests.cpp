#include "stdafx.h"
using namespace Checkmate;


TEST(MoveListTests,CheckMoveExtraction)
{
	Represenation& rep = *(new Represenation());
	MoveListBase* mls = CreateNewMoveList();
	MoveInfo* mi = new MoveInfo(WHITE,SQ_A1,(setSquare(SQ_H3) | setSquare(SQ_F3)), KNIGHT, NORMAL,rep);
	mls->Append(*mi);
	EXPECT_EQ(2, mls->length());
}

TEST(MoveListTests, CheckMoveExtractionThroughOperator)
{
	Represenation& rep = *(new Represenation());
	MoveListBase* mls = CreateNewMoveList();
	MoveInfo* mi = new MoveInfo(WHITE, SQ_A1,(setSquare(SQ_H3) | setSquare(SQ_F3)), KNIGHT, NORMAL, rep);
	mls->Append(*mi);
	EXPECT_EQ(2, mls->length());
}

