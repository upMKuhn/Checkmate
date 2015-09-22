#pragma once
#include "stdafx.h"

namespace Checkmate {
		
	/// <summary>
	/// The Purpose of the PiecePlacement is a 
	/// instruction structure, to position pieces. 
	/// </summary>
	struct PiecePlacement
	{
		Square position;
		Piece piece;

		PiecePlacement()
		{
			int i = 5;
			i++;
		}
		PiecePlacement(Square position, Piece piece);

		static PiecePlacement create();

		bool isValid() {
			return position != SQUARE_NB || piece != NO_PIECE;
		}

		friend ::std::ostream& operator<<(::std::ostream& stream, const PiecePlacement& instruction);
	};
}