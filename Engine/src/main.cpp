#include "stdafx.h"

using namespace Checkmate;

void init()
{
	Bitboards::init();
}

int main(int argc, char* argv[]) {
	
	using namespace Checkmate; using namespace DataStructs; using namespace std;

	init();
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::cout << "Hello World" << std::endl;
	
	string token, cmd;
	for (int i = 1; i < argc; ++i)
		cmd += string(argv[i]) + " ";

	if (argc == 1 && !getline(cin, cmd)) // Block here waiting for input or EOF
		cmd = "quit";

	std::cout << cmd << endl;

	system("PAUSE");
	return 0;
}
