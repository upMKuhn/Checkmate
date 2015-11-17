#include "stdafx.h"

using namespace Checkmate;

void init()
{
	Bitboards::init();
	Zorbist::init();

}



void performanceTest()
{
	string startFen = "rnbqkbnr/P7/8/5PpP/8/4p3/4pP1P/RNBQK2R w KQkq G5 0 1";
	Represenation* board = new Represenation(startFen);
	
	MoveGen movegen = MoveGen(*board);
	MoveList* mvlist;

	long long time = 0;
	int count = 0;
	int total = 0;

	const int stoppAt = 100000000;
	while (count < stoppAt && total < stoppAt)
	{
		
		if(Checkmate::system_time_to_msec() - time >= 1000)
		{
			total += count;
			std::cout << "per sec: " << count << " total: " << total << std::endl;
			time = Checkmate::system_time_to_msec();
			count = 0;
		}

		movegen.generate_all(WHITE);
		mvlist = movegen.m_moveGeninfo.move_list;
		count += mvlist->index;
		
		while (mvlist->index > 0)
		{
			board->makeMove(mvlist->m);
			MoveList* todelete = mvlist;
			mvlist = mvlist->node;
			delete todelete;
			board->undoMove();
		}
	}
	std::cout << "END PERFORMANCE TEST " << std::endl;
}


void ShowBoard(Color c)
{
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	string token, cmd;
	string startFen =  c == WHITE ?"rnbqkbnr/P7/8/5PpP/8/4p3/4pP1P/RNBQK2R w KQkq G5 0 1" 
							: "rnbqkbnr/P7/8/5PpP/4Q3/4p3/4pP1P/RNBQK2R b KQkq G5 0 1";
	Represenation& board = *(new Represenation(startFen));
	Represenation& br = *(new Represenation(board));
	Represenation* bp = new Represenation(board);



	MoveList* mvlist = new MoveList();
	MoveGen& movgen = *(new MoveGen(board));

	mvlist = movgen.generate_all(c).move_list;
	
	std::cout << "to move: " << (c == WHITE ? "WHITE" : "BLACK") << std::endl;

	while(mvlist->index > 0)
	{
		board.makeMove(mvlist->m);
		std::cout << board.to_string() << std::endl;
		mvlist = mvlist->node;
		board.undoMove();
		getline(cin, cmd);
	}

}

int main(int argc, char* argv[]) {
	
	init();
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	string token, cmd;
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//performanceTest();
	ShowBoard(BLACK);
	ShowBoard(WHITE);
	
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
