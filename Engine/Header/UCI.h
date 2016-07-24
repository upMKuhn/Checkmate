#pragma once
#include "stdafx.h";

namespace Checkmate {
	class UCI
	{



	public:
		UCI();
		~UCI();

		void Start();
		void WaitForTermination();
		void Stop();

	protected:
		void loop();
		
		//helper
		bool isCommand(string input, string cmd);
		
		//commands
		void EvaluateCmd(string commandLine);
		
		void SetPosition(string commandLine);
		
		void Search(string commandLine);

		void TestCmd(string commandLine);

		void Movegen(string commandLine);

		void ShowBoard();


		string removeWord(string line, string word);

		bool hasStarted = false;
		Represenation* board;
	};

}