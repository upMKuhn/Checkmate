#include "stdafx.h"
#include <boost\algorithm\algorithm.hpp>
#include <boost\algorithm\string.hpp>
using namespace Checkmate;
using namespace boost::algorithm;

UCI::UCI()
{
	board = new Represenation();
}


UCI::~UCI()
{

}

void UCI::Start()
{
	if (hasStarted) return;
	loop();
}

void UCI::Stop()
{
}

void UCI::WaitForTermination()
{
}


void UCI::loop() {
	using namespace Checkmate; using namespace std;
	string cmd = "" ;
	while (getline(cin, cmd) && cmd != "quit")
	{
		if (isCommand(cmd, "evaluate"))
			EvaluateCmd(cmd);
		if (isCommand(cmd, "set-position"))
			SetPosition(cmd);
		if (isCommand(cmd, "show"))
			ShowBoard();
		if (isCommand(cmd, "go"))
			Search(removeWord(cmd,"go"));
		if (isCommand(cmd, "test"))
			TestCmd(removeWord(cmd, "test"));
		if (isCommand(cmd, "movegen"))
			Movegen(removeWord(cmd, "test"));

		if (isCommand(cmd, "perft"))
			Perft(removeWord(cmd, "perft"));
	}

}

bool Checkmate::UCI::isCommand(string input, string cmd)
{
	return starts_with<string, string>(input, cmd);
}

void UCI::EvaluateCmd(string commandLine)
{
	Bitboard bb =  board->colorbb[0];
	MoveGen movegen(*board);
	Bitboard bb1 = board->colorbb[0];
	MoveGenInfo mgi;
	Evaluation eval(board, &movegen.generate_all());
	Bitboard bb2 = board->colorbb[0];

	Color orgUs = board->sideToMove;
	Score usScore = eval.Evaluate(orgUs);
	eval.print(orgUs, usScore);
	Score themScore = eval.Evaluate(~orgUs);
	eval.print(~orgUs, themScore);

	Score relativeScore = usScore - themScore;

	std::cout << "score " << relativeScore << endl;
	Bitboard bb3 = board->colorbb[0];
}

void UCI::SetPosition(string commandLine)
{
	vector<string> params;

	try {
		board->fenToBoard(removeWord(commandLine, "set-position"));
	}
	catch(exception e){
	}
}

void UCI::Search(string commandLine)
{

	int depth;
	try
	{
		depth = boost::lexical_cast<int>(commandLine);
	}
	catch (boost::bad_lexical_cast& e)
	{
		depth = 6;
	}
	PrincipleVariationSearch abs(board, depth);
	ScoredMove* sm  = abs.Search();
	std::cout << "best move:" << move_tostring(sm->move) << " - " << sm->score << endl;
		std::cout << "ponder:" << move_tostring(abs.GetBestMove(depth-1)->move) << " - " << abs.GetBestMove(depth - 1)->score << endl;

	delete sm;
}

void UCI::Movegen(string commandLine)
{
	MoveGenInfo mgi;
	MoveGen gen(*board);
	MoveListBase* list = gen.generate_all().move_list;
	while (list->hasNext())
	{
		board->makeMove(list->getNextOrDefault());
		ShowBoard();
		getline(cin, commandLine);
		board->undoMove();
	}

}

void UCI::Perft(string commandLine)
{
	int depth;
	try
	{
		depth = boost::lexical_cast<int>(commandLine);
	}
	catch (boost::bad_lexical_cast& e)
	{
		depth = 6;
	}
	
	PrincipleVariationSearch pv(board, depth);
	INT64 startAt = system_time_to_msec();
	INT64 nodes = pv.Perft(depth);
	INT64 timeTaken = system_time_to_msec() - startAt;
	cout << "=======================" << endl;
	cout << "Total time(ms) " << timeTaken << endl;
	cout << "Nodes searched: " << nodes << endl;
	cout << "Nodes / second: " << 1000 * nodes / timeTaken << endl;

}

void UCI::TestCmd(string commandLine)
{
}

void UCI::ShowBoard()
{
	using namespace Checkmate; using namespace DataStructs; using namespace std;
	MoveGen& movgen = *(new MoveGen(*board));
	MoveGenInfo mgi;
	MoveListBase* mvlist = CreateNewMoveList();
	Evaluation eval = Evaluation(board, &movgen.getMoveGenInfo());
	Score our, their;
	string token, cmd;
	Move m;
	Color c = board->sideToMove;

	mvlist = movgen.generate_all(c).move_list;

	std::cout << board->to_string() << std::endl;
	std::cout << "to move: " << color_tostring(c) << std::endl;
	our = eval.Evaluate(c);
	their = eval.Evaluate(~c);
	std::cout << "Score: " << our - their << endl;
	std::cout << " " << color_tostring(c) << " " << our << " "
		<< color_tostring(~c) << " " << their << std::endl;
}

string UCI::removeWord(string line, string word)
{
	string lowerstr = line;
	to_lower(lowerstr);
	to_lower(word);
	int start = lowerstr.find_first_of(word);
	return ltrim(line.substr(start + word.length()));
}


