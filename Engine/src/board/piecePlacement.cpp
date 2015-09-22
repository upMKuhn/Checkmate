#include "stdafx.h"

/// <summary>
/// The Purpose of the PiecePlacement is a 
/// instruction structure, to position pieces. 
/// </summary>

namespace Checkmate {

	PiecePlacement::PiecePlacement(Square position, Piece piece)
	{
		this->piece = piece;
		this->position = position;
	}

	PiecePlacement PiecePlacement::create()
	{
		return PiecePlacement(SQUARE_NB, NO_PIECE);
	}

	//toString
	::std::ostream& operator<<(::std::ostream& stream, const PiecePlacement& instruction)
	{
		char FileLookup[] = { 'A','B','C','D','E','F','G','H' };
		char file = FileLookup[file_of(instruction.position)];
		int rank = rank_of(instruction.position)+1;
		stream << "PiecePlacement: ("<< " Piece:" << instruction.piece << ", Square: "  << file << rank << " )";
		return stream;
	}
	
};