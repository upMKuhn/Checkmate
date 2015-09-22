#include "stdafx.h"

using namespace Checkmate;

void init()
{
	Bitboards::init();
}

int main(int argc, char* argv[]) {
	
	using namespace Checkmate; using namespace DataStructs;
	init();
	
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	system("PAUSE");
	return 0;
}
