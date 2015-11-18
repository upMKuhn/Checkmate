#pragma once
#include "stdafx.h"

namespace Checkmate {
		
	/// <summary>
	/// The Purpose of the PiecePlacement is a 
	/// instruction structure, to position pieces. 
	/// </summary>
	struct PiecePlacement
	{
		Square position = SQ_NONE;
		Piece piece = NO_PIECE;
		int index = 0;
		PiecePlacement* next;

		PiecePlacement(){}
		PiecePlacement(Square position, Piece piece);

		static PiecePlacement create();

		inline PiecePlacement* append()
		{
			
		}

		bool isValid() {
			return position != SQUARE_NB || piece != NO_PIECE;
		}

		friend ::std::ostream& operator<<(::std::ostream& stream, const PiecePlacement& instruction);
	};
}