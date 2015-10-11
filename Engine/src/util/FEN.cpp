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

	void FEN_Parser::init()
	{
		sideToMove = NO_COLOR;
		enPassant = SQ_NONE;
		halfMoveClock = -1;
		FullMoveClock = -1;
	}

	void FEN_Parser::nextInstruction(PiecePlacement *&pTemp)
	{
		m_boardParser->nextInstruction(pTemp);
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
		int startOfState = getEndOfRepresenation();

		extractSideToMove(startOfState);
		extractCastelingRights(startOfState);
		extractEnPassant(startOfState);
		extractMoveCounter(startOfState);

	}

	void FEN_Parser::extractSideToMove(int& index)
	{
		if(tolower(m_FEN[index]) == 'w')
		{
			sideToMove = WHITE;
		}
		if (tolower(m_FEN[index]) == 'b')
		{
			sideToMove = BLACK;
		}
		jumpSpaceChars(++index);
	}

	void FEN_Parser::extractCastelingRights(int& index)
	{
		bool Casteling = true;
		//While End Of
		while (m_FEN[index] != ' ' && index < m_FEN.length() && Casteling)
		{
			switch (m_FEN[index])
			{
			case 'k':
				castlingRights |= WHITE_OO;
				break;
			case 'q':
				castlingRights |= WHITE_OOO;
				break;
			case 'K':
				castlingRights |= BLACK_OO;
				break;
			case 'Q':
				castlingRights |= BLACK_OOO;
				break;
			default:
				Casteling = false;
				index--;
			}
			index++;
		}
		jumpSpaceChars(index);
	}

	void FEN_Parser::extractEnPassant(int& index)
	{
		char file = tolower(m_FEN[index++]);
		if(file != '-' && ('a' <= file && file <= 'h'))
		{
			int rank = std::stoi(m_FEN.substr(index,1))-1;
			File myFile = (File)(file - 'a');
			enPassant = make_square(myFile, (Rank)rank);
		}else
		{
			enPassant = SQ_NONE;
		}
		jumpSpaceChars(++index);
	}
	
	void FEN_Parser::extractMoveCounter(int& index)
	{
		
		int endOfNumber = m_FEN.find_first_of(' ', index);
		if (endOfNumber > 0) {
			halfMoveClock = std::stoi(m_FEN.substr(index, endOfNumber - index));
			index = endOfNumber + 1;
		}
		
		FullMoveClock = std::stoi(m_FEN.substr(index, m_FEN.length()));
		jumpSpaceChars(index);
	}


	int FEN_Parser::getEndOfRepresenation()
	{
		
		for (int i = 0; i < m_FEN.length()-1; i++)
		{
			if(m_FEN[i] == ' ')
			{
				return i+1;
			}
		}
	}

	void FEN_Parser::jumpSpaceChars(int& index)
	{
		while (m_FEN[index] == ' ' && index < m_FEN.length())
		{
			index++;
		}
	}
	

	

	

	



}
