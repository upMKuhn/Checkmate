#pragma once

#include <iostream>
#include "..\util\types.h"
namespace Checkmate {
	

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