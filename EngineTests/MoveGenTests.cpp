#include "stdafx.h"
using namespace Checkmate;


TEST(MoveGenTests, QuietRookMovesGeneratedAccuratly)
{
	Represenation board("K7/8/8/2kN4/2Nr1N2/8/3N4/8 b - - 0 1");
	MoveGen mg = MoveGen(board);
	MoveGenInfo mgi = mg.generate_all(BLACK);
	EXPECT_EQ(1, 2);
}