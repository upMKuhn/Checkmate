#include "stdafx.h"
#include "FEN_TEST.h"


FEN_TEST::FEN_TEST()
{
}


FEN_TEST::~FEN_TEST()
{
}

Bitboard FEN_TEST::parsePieceBoard(Piece board[])
{
	Bitboard bb = 0;
	for (Square sqr = SQ_A1; sqr < SQUARE_NB; ++sqr)
	{
		 if(board[sqr] != NO_PIECE)
		 {
			 bb |= (1ULL << sqr);
		 }
	}

	return bb;
	
}

void FEN_TEST::ASSERT_boardParser(FEN_TEST_DATA * testData, FEN_Parser * parser)
{
	PiecePlacement *instruct;
	parser->nextInstruction(instruct);

	Bitboard bb = 0;
	
	while (instruct != NULL)
	{
		bb |=  (1ULL << instruct->position);
		ASSERT_EQ(testData->board[instruct->position], instruct->piece);
		parser->nextInstruction(instruct);
	}
	

	Bitboard parseBB = parsePieceBoard(testData->board);
	ASSERT_EQ(parsePieceBoard(testData->board), bb);
}



TEST_F(FEN_TEST, CHECK_FEN_RESULT)
{
	using namespace Checkmate;

	FEN_TEST_DATA data("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	data.setPiece(W_ROOK, SQ_A1); data.setPiece(W_PAWN, SQ_A2);   data.setPiece(B_ROOK, SQ_A8); data.setPiece(B_PAWN, SQ_A7);
	data.setPiece(W_KNIGHT, SQ_B1); data.setPiece(W_PAWN, SQ_B2); data.setPiece(B_KNIGHT, SQ_B8); data.setPiece(B_PAWN, SQ_B7);
	data.setPiece(W_BISHOP, SQ_C1); data.setPiece(W_PAWN, SQ_C2); data.setPiece(B_BISHOP, SQ_C8); data.setPiece(B_PAWN, SQ_C7);
	data.setPiece(W_QUEEN, SQ_D1); data.setPiece(W_PAWN, SQ_D2);  data.setPiece(B_QUEEN, SQ_D8); data.setPiece(B_PAWN, SQ_D7);
	data.setPiece(W_KING, SQ_E1); data.setPiece(W_PAWN, SQ_E2);   data.setPiece(B_KING, SQ_E8); data.setPiece(B_PAWN, SQ_E7);
	data.setPiece(W_BISHOP, SQ_F1); data.setPiece(W_PAWN, SQ_F2); data.setPiece(B_BISHOP, SQ_F8); data.setPiece(B_PAWN, SQ_F7);
	data.setPiece(W_KNIGHT, SQ_G1); data.setPiece(W_PAWN, SQ_G2); data.setPiece(B_KNIGHT, SQ_G8); data.setPiece(B_PAWN, SQ_G7);
	data.setPiece(W_ROOK, SQ_H1); data.setPiece(W_PAWN, SQ_H2);   data.setPiece(B_ROOK, SQ_H8); data.setPiece(B_PAWN, SQ_H7);


	FEN_Parser parser(data.fen);
	ASSERT_boardParser(&data, &parser);

	parser.process();


	
}