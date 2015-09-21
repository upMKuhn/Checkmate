#include "FEN.h"

/// <summary>
/// The class needs to be used in a procedural manner
/// first get nextPiece until no exsist. The you can access stuff like
/// Castleing rights
/// </summary>
namespace Checkmate {
#define NEXT_RANK '/'
#define JUMP_FILES(x) isdigit(x)
#define END_OF_BOARD ' '
	
	
	FEN_Phraser::FEN_Phraser(::std::string FEN)
	{
		this->m_FEN = FEN;
		this->m_squareAt = SQ_A8;
		this->m_charAt = 0;

		process();
	}

	FEN_Phraser::~FEN_Phraser()
	{
	}

	void FEN_Phraser::process()
	{
		extractBoard();
	}

	void FEN_Phraser::nextInstruction(PiecePlacement *&pTemp)
	{
		if (hasNextInstruction())
		{
			using DataStructs::DoubleLinkedListNode;

			DoubleLinkedListNode<PiecePlacement>* 
			node = m_boardInstruction.getNode(instructionAt);
			pTemp = &(node->getValue());
			instructionAt++;
		}
		else {
			pTemp = NULL;
		}
			
	}

	void FEN_Phraser::extractBoard()
	{
		while (hasPiece())
		{
			PiecePlacement temp = nextPiece();
			m_boardInstruction.appendHead(temp);
		}
	}

	void FEN_Phraser::jumpFile(int jmp)
	{
		File currentFile = file_of(m_squareAt);
		Rank currentRank = rank_of(m_squareAt);

		currentFile += ((File)(jmp-1));
		assert((currentFile <= FILE_H));
		m_squareAt = make_square(currentFile, currentRank);
	}

	void FEN_Phraser::nextRank()
	{
		--m_squareAt;
		Rank currentRank = rank_of(m_squareAt);
		m_squareAt = make_square(FILE_A, --currentRank);
		assert(!(m_squareAt > SQ_H8 || m_squareAt < SQ_A1));
	}

	bool FEN_Phraser::hasPiece()
	{
		return (bool)(m_FEN.size() > m_charAt && m_FEN.at(m_charAt) != END_OF_BOARD);
	}

	PieceType FEN_Phraser::toPiece(char piece)
	{
		piece = tolower(piece);
		switch ( piece)
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

	PiecePlacement FEN_Phraser::nextPiece()
	{
		char currentChar = m_FEN.at(m_charAt);
		PiecePlacement phrase;
			
		if(NEXT_RANK == currentChar)
		{
			nextRank();
			m_charAt++;
			phrase = nextPiece();
		}

		if (JUMP_FILES(currentChar) )
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

	bool FEN_Phraser::hasNextInstruction()
	{
		return m_boardInstruction.Length() > instructionAt;
	}

	

	



}
