#pragma once
#include "stdafx.h"
namespace Checkmate {
	namespace DS = DataStructs;
	
	enum CasualList { CAPTURES, NORMALS, CASUAL_LIST_NB };
	enum SpecialList { CHECKS, ENPASSANTS, CASTELING, PROMOTIONS, SPECIAL_LIST_NB };
	ENABLE_FULL_OPERATORS_ON(CasualList)
	ENABLE_FULL_OPERATORS_ON(SpecialList)

/// <summary>
/// This class is repsonsible for keeping Track of moves. 
/// I decieded to make this class as complex it is right now in
/// order to provide a simply sorted move storage 
/// 
/// The advantage of such data type is, it should be capable of deleting
/// moves stored, in order to update the list with the latest information,
/// which reduced the load of the move generator!
/// </summary>
class ChachedMoveList : public MoveListBase{
		
	private:
		int m_lenght = 0;

		static const char SPECIAL_LS_MAX = 8;
		static const char CASUAL_LS_MAX = 16;
		
		DS::SinglyLL<Move>* m_casualList[CASUAL_LIST_NB][COLOR_NB][PIECE_TYPE_NB][CASUAL_LS_MAX];
		DS::SinglyLL<Move>* m_specialList[SPECIAL_LIST_NB][COLOR_NB][SPECIAL_LS_MAX];

		int m_CasualIndex[SQUARE_NB];
		int m_SpecialIndex[SPECIAL_LIST_NB][SQUARE_NB];
		int m_SpecialCount[SPECIAL_LIST_NB];
		int m_PieceCount[COLOR_NB][PIECE_TYPE_NB];
		Piece m_PieceIndex[SQUARE_NB];
		void init();

		void put_piece(Square s, Piece p);
		void remove_piece(Square s);
		void move_piece(Square from, Square to);

		void appendCasualList(MoveInfo & mi, CasualList cs, Square s, Move m);
		void appendSpecialList(MoveInfo & mi, SpecialList sl, Square s, Move m);


		//GetNext Utillity
		bool specialListDone = false;
		bool casualListDone = false;
		int specialIndexAt = 0;
		int casualIndexAt = 0;
		PieceType casualListPieceTypeAt = PAWN;
		SpecialList specialTypeAt = SpecialList::CHECKS ;
		CasualList casualTypeAt = CasualList::CAPTURES;

		DS::SinglyLL<Move>* ListAt = nullptr;
		DS::SinglyLL<Move>* getNextNonEmptySpecialList(Color c);
		DS::SinglyLL<Move>* getNextNonEmptyCasuallList(Color c);
	public:

		ChachedMoveList();
		virtual void Append(MoveInfo& mi) override;

		virtual Move getNextOrDefault(Color c) override;
		virtual void ResetGetNext() override;
		virtual void OnGeneratingNewMoves(Move m) abstract;
		virtual bool hasNext() override;
		virtual int length() override;
	};

	

}

