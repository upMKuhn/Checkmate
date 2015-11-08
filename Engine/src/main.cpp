#include "stdafx.h"

using namespace Checkmate;

void init()
{
	Bitboards::init();
	
}

void ShowBoard()
{
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	string token, cmd;
	string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Represenation* board = new Represenation("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	
	MoveList* mvlist = new MoveList(*board);
	MoveGen* movgen = new MoveGen();
	mvlist = movgen->generate_pawnMoves(WHITE, mvlist);

	while(mvlist->index > 0)
	{
		board->makeMove(mvlist->m);
		std::cout << board->to_string() << std::endl;
		mvlist = mvlist->node;
		board->fenToBoard(startFen);
		getline(cin, cmd);
	}

	//std::cout << Bitboards::print( generate_pawnMoves ) << std::endl;
	
	
}

int main(int argc, char* argv[]) {
	
	init();
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	string token, cmd;
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	ShowBoard();
	std::cout << board.to_string() << std::endl;
	system("PAUSE");
	for (int i = 1; i < argc; ++i)
		cmd += string(argv[i]) + " ";

	if (argc == 1 && !getline(cin, cmd)) // Block here waiting for input or EOF
		cmd = "quit";

	std::cout << cmd << endl;

	system("PAUSE");
	return 0;
}
