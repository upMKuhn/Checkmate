#pragma once
#include "stdafx.h"

namespace Checkmate
{
	inline MoveListBase* CreateNewMoveList()
	{
		return new SimpleMoveList();
	}
}