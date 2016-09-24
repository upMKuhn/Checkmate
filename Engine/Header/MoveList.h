#pragma once
#include "stdafx.h"

#ifndef MOVE_LS_H
#define MOVE_LS_H

namespace Checkmate
{
	enum MoveListType {
		LS_RARE,
		LS_ATTACKS,
		LS_LEGAL
	};

	enum MoveListType_MAX {
		LS_RARE_MAX = 20,
		LS_ATTACKS_MAX = 50,
		LS_LEGAL_MAX = 100
	};

	
	struct MoveList {

		MoveList(Represenation* rep)
		{
			RareMovesLs_Last = &RareMovesLs[0];
			AttacksMovesLs_Last = &AttacksMovesLs[0];
			LegalMovesLs_Last = &LegalMovesLs[0];
			generate_all(*rep, LegalMovesLs_Last, AttacksMovesLs_Last);
			ResetNext();
		}

		void Append(Move m)
		{
			*(LegalMovesLs_Last++) = m;
		}


		inline ScoredMove* Next()
		{
			if (at == nullptr) 
				return nullptr;
			if (at == RareMovesLs_Last) 
				at = &AttacksMovesLs[0];
			if (at == AttacksMovesLs_Last) 
				at = &LegalMovesLs[0];
			if (at == LegalMovesLs_Last) 
				at = nullptr;
			return at != nullptr ? at++ : nullptr;
		}

		inline void ResetNext()
		{
			at = &RareMovesLs[0];
		}

		inline int size()
		{
			return (RareMovesLs_Last - RareMovesLs) + (AttacksMovesLs_Last - AttacksMovesLs) + (LegalMovesLs_Last - LegalMovesLs);
		}
		
		ScoredMove RareMovesLs[LS_RARE_MAX];
		ScoredMove AttacksMovesLs[LS_ATTACKS_MAX];
		ScoredMove LegalMovesLs[LS_LEGAL_MAX];

		ScoredMove* at = nullptr;
		ScoredMove* RareMovesLs_Last; 
		ScoredMove* AttacksMovesLs_Last;
		ScoredMove* LegalMovesLs_Last;

	};


}

#endif