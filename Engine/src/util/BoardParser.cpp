#include "stdafx.h"

namespace Checkmate {

#define NEXT_RANK '/'
#define JUMP_FILES(x) isdigit(x)
#define END_OF_BOARD ' '

	BoardParser::BoardParser(::std::string fen)
	{
		this->m_FEN = fen;
		this->m_squareAt = SQ_A8;
		this->m_charAt = 0;

	}

	BoardParser::~BoardParser()
	{
	}
	
	

	void BoardParser::extractBoard()
	{
		while (hasPiece())
		{
			PiecePlacement temp = nextPiece();
			m_boardInstruction.appendHead(temp);
		}
	}

	void BoardParser::nextInstruction(PiecePlacement *&pTemp)
	{
		
		if (hasNextInstruction())
		{
			pTemp = &m_boardInstruction.getNode(m_instructionAt++)->getValue();
		}else
		{
			pTemp = NULL;
		}
	}

	/// <summary>
	/// requires extract board to be called first
	/// Determines whether [has next instruction]
	/// And update pointer passed by reference.
	/// </summary>
	/// <returns></returns>
	bool BoardParser::hasNextInstruction()
	{
		return m_instructionAt < m_boardInstruction.Length()+1;
	}




	bool BoardParser::hasPiece()
	{
		return (bool)(m_FEN.size() > m_charAt && m_FEN.at(m_charAt) != END_OF_BOARD);
	}

	PiecePlacement BoardParser::nextPiece()
	{
		char currentChar = m_FEN.at(m_charAt);
		PiecePlacement phrase;

		if (NEXT_RANK == currentChar)
		{
			nextRank();
			m_charAt++;
			phrase = nextPiece();
		}

		if (JUMP_FILES(currentChar))
		{
			jumpFile(currentChar - '0');
			m_charAt++;
			phrase = nextPiece();
		}

		if (toPiece(currentChar) != NO_PIECE_TYPE)
		{
			Color c = isupper(currentChar) ? WHITE : BLACK;
			phrase.piece = make_piece(c, toPiece(currentChar));
			phrase.position = m_squareAt;
			++m_squareAt;
			m_charAt++;
		}
		return phrase;
	}

	//Details Details Details

	void BoardParser::jumpFile(int jmp)
	{
		File currentFile = file_of(m_squareAt);
		Rank currentRank = rank_of(m_squareAt);

		currentFile += ((File)(jmp - 1));
		assert((currentFile <= FILE_H));
		m_squareAt = make_square(currentFile, currentRank);
	}

	void BoardParser::nextRank()
	{
		--m_squareAt;
		Rank currentRank = rank_of(m_squareAt);
		m_squareAt = make_square(FILE_A, --currentRank);
		assert(!(m_squareAt > SQ_H8 || m_squareAt < SQ_A1));
	}

	PieceType BoardParser::toPiece(char piece)
	{
		piece = tolower(piece);
		switch (piece)
		{
		case 'r':
			return ROOK;
		case 'n':
			return KNIGHT;
		case 'b':
			return BISHOP;
		case 'q':
			return QUEEN;
		case 'k':
			return KING;
		case 'p':
			return PAWN;
		default:
			return NO_PIECE_TYPE;
		}
	}


	void createBoardParser(BoardParser *pointer, ::std::string newFen)
	{
		if (pointer != NULL)
		{
			delete pointer;
			pointer = new BoardParser(newFen);
		}
	}
	
}