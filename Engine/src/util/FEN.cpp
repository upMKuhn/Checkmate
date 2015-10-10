#include "stdafx.h"

/// <summary>
/// The class needs to be used in a procedural manner
/// first get nextPiece until no exsist. The you can access stuff like
/// Castleing rights
/// </summary>
namespace Checkmate {

	
	
	FEN_Parser::FEN_Parser(::std::string FEN)
	{
		this->m_FEN = FEN;
		m_boardParser = new BoardParser(m_FEN);
		process();
	}

	FEN_Parser::~FEN_Parser()
	{
		delete m_boardParser;
	}

	void FEN_Parser::process()
	{
		if (m_boardParser == NULL)
		{
			m_boardParser = new BoardParser(m_FEN);
		}

		m_boardParser->extractBoard();
		extractStates();
	}

	

	void FEN_Parser::extractStates()
	{

	}

	
	void FEN_Parser::nextInstruction(PiecePlacement *&pTemp)
	{
		m_boardParser->nextInstruction(pTemp);
	}


	

	



}
