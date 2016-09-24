#include "stdafx.h"

using namespace Checkmate;

void init()
{
	Bitboards::init();
	Zorbist::init();

}

void ShowBoard(Color c, string fen)
{
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	Represenation& board = *(new Represenation(fen));
	MoveGen& movgen = *(new MoveGen(board));
	MoveListBase* mvlist = CreateNewMoveList();
	Evaluation eval = Evaluation(&board, &movgen.getMoveGenInfo());
	Score our, their;
	string token, cmd;
	Move m;

	mvlist = movgen.generate_all(c).move_list;
	
	while(m = mvlist->getNextOrDefault(), m != MOVE_NONE)
	{
		board.makeMove(m);
		std::cout << board.to_string() << std::endl;
		std::cout << "to move: " << (c == WHITE ? "WHITE" : "BLACK") << std::endl;
		our = eval.Evaluate(c);
		their = eval.Evaluate(~c);
		std::cout << "Score: " << our - their << std::endl;
		board.undoMove();
		getline(cin, cmd);
	}
	mvlist->ResetGetNext();

}


#ifndef UNIT_TEST

int main(int argc, char* argv[]) {
	init();

	UCI uci;
	uci.Start();
	return 0;
}

#endif