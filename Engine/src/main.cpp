#include "stdafx.h"

using namespace Checkmate;

void init()
{
	Bitboards::init();
	Zorbist::init();

}

void performanceTest()
{
	string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Represenation* board = new Represenation(startFen);
	MoveList* mvlist = new MoveList(*board);
	MoveGen* movgen = new MoveGen();

	long long time = 0;
	int count = 0;
	int total = 0;
	while (total < 100000000)
	{
		
		if(Checkmate::system_time_to_msec() - time >= 1000)
		{
			total += count;
			std::cout << "per sec: " << count << " total: " << total << std::endl;
			time = Checkmate::system_time_to_msec();
			count = 0;
		}

		count += mvlist->index;

		while (mvlist->index > 0)
		{
			board->makeMove(mvlist->m);
			MoveList* todelete = mvlist;
			mvlist = mvlist->node;
			delete todelete;
			board->undoMove();
		}
		delete mvlist;
		mvlist = new MoveList(*board);
		mvlist = movgen->generate_all(WHITE, mvlist, *board);
	}
}


void ShowBoard()
{
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	string token, cmd;
	string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	Represenation* board = new Represenation(startFen);
	
	MoveList* mvlist = new MoveList(*board);
	MoveGen* movgen = new MoveGen();

	mvlist = movgen->generate_all(WHITE, mvlist, *board);
	

	while(mvlist->index > 0)
	{
		board->makeMove(mvlist->m);
		std::cout << board->to_string() << std::endl;
		mvlist = mvlist->node;
		board->undoMove();
		getline(cin, cmd);
	}

}

int main(int argc, char* argv[]) {
	
	init();
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	string token, cmd;
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	performanceTest();
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
