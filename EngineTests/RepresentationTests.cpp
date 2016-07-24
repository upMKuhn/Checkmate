#include "stdafx.h"
#include "RepresentationTests.h"
using namespace Checkmate;

RepresentationTests::RepresentationTests()
{
	Checkmate::Bitboards::init();
	Checkmate::Zorbist::init();
}


RepresentationTests::~RepresentationTests()
{
}

TEST_F(RepresentationTests, MoveRooks_CheckCastelingRevoked)
{
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 1";
	Represenation board(fen);
	Move mvQueenRook = make<NORMAL>(SQ_A1, SQ_A3,ROOK);
	Move mvKingRook = make<NORMAL>(SQ_H1, SQ_H3,ROOK);
	board.makeMove(mvQueenRook);
	EXPECT_EQ((BLACK_OO | BLACK_OOO | WHITE_OO), board.castlingRights);
	board.sideToMove = ~board.sideToMove;
	board.makeMove(mvKingRook);
	EXPECT_EQ((BLACK_OO | BLACK_OOO), board.castlingRights);
}

TEST_F(RepresentationTests, MovePawnCheckEnpassant)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move mv = make<NORMAL>(SQ_A2, SQ_A4,PAWN);
	board.makeMove(mv);
	EXPECT_EQ(SQ_A4, board.enPassant);
}

TEST_F(RepresentationTests, MakeNormalMove_CheckPieceWasMoved_UndoAndCheck_FEN)
{
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 1";
	Represenation board(fen);
	Move mv = make<NORMAL>(SQ_B1, SQ_C3, KNIGHT);
	board.makeMove(mv);
	EXPECT_EQ(board.boardToFEN(), "rnbqkbnr/pppppppp/8/8/8/2N5/8/R1BQKBNR b KQkq - 1 1");
	EXPECT_EQ(W_KNIGHT, board.piece_on(SQ_C3));
	board.undoMove();
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MakeCapture_CheckPieceWasTaken_UndoCheck_FEN)
{
	string fen = "rnb1kbnr/pppppppp/8/8/4q3/8/8/RNBQKBNR w KQkq - 0 1";
	Represenation board(fen);
	Move mv = make<CAPTURE>(SQ_D1, SQ_D4, QUEEN);
	board.makeMove(mv);
	EXPECT_EQ(W_QUEEN, board.piece_on(SQ_D4)); 
	board.undoMove();
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MakeCastelin_CheckMoveWasCarriedOut_UndoMoveVerify_FEN)
{
	string fen = "rnb1kbnr/pppppppp/8/8/4q3/8/8/R3K2R w KQkq - 0 1";
	Represenation board(fen);
	Move mv = make<CASTLING>(SQ_E1, SQ_A1,KING);
	board.makeMove(mv);
	EXPECT_EQ(W_KING, board.piece_on(SQ_C1)); //Prove FEN must have changed
	EXPECT_EQ(W_ROOK, board.piece_on(SQ_D1)); //Prove FEN must have changed
	EXPECT_EQ(NO_CASTLING, board.can_castle(WHITE));
	board.undoMove();
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MakePromotionWithCapture_CheckPromotedCorrectly_UndoMoveVerify_FEN)
{
	string fen = "rnb1kbnr/P7/8/8/4q3/8/8/R3K2R w KQkq - 0 1";
	Represenation board(fen);
	Move mv = make_promotion_withCapture_move(SQ_A7, SQ_B8, PAWN,QUEEN,KNIGHT);
	board.makeMove(mv); 
	EXPECT_EQ(W_QUEEN, board.piece_on(SQ_B8));
	board.undoMove();
	EXPECT_EQ(B_KNIGHT, board.piece_on(SQ_B8));
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MakeEnpassantMove_CheckFEN_UndoCheckFEN)
{
	string fen = "rnbqkbnr/P7/8/5PpP/8/4p3/4pP1P/RNBQK2R w KQkq g5 0 1";
	string expect = "rnbqkbnr/P7/6P1/5P2/8/4p3/4pP1P/RNBQK2R b KQkq - 1 0";
	Represenation board(fen);
	Move mv = make<ENPASSANT>(SQ_H5, SQ_G6, PAWN,PAWN);
	board.makeMove(mv);
	
	EXPECT_EQ(W_PAWN, board.piece_on(SQ_G6)); 
	EXPECT_EQ(NO_PIECE, board.piece_on(SQ_G5));
	EXPECT_EQ(expect, board.boardToFEN());
	board.undoMove();
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MoveWhiteAndBLACK_UndoOne_CheckFen)
{
	string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	string expectFen = "rnbqkbnr/pppppppp/8/8/8/5N2/PPPPPPPP/RNBQKB1R b KQkq - 1 1";
	Represenation board(startFen);
	Move mv = make<NORMAL>(SQ_G1, SQ_F3, KNIGHT);
	Move mv2 = make<NORMAL>(SQ_G7, SQ_G5,PAWN);
	board.makeMove(mv);
	board.makeMove(mv2);
	EXPECT_EQ(B_PAWN, board.piece_on(SQ_G5));

	board.undoMove();
	EXPECT_EQ(expectFen , board.boardToFEN());
	EXPECT_EQ(B_PAWN, board.piece_on(SQ_G7));
}

TEST_F(RepresentationTests, InitiateBoard_CheckAllPieceLocations)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	int expectCount[PIECE_NB]; ::std::fill_n(expectCount, PIECE_NB, NO_PIECE);;
	expectCount[W_ROOK] = 2; expectCount[W_KNIGHT] = 2; expectCount[W_BISHOP] = 2;
	expectCount[W_QUEEN] = 1; expectCount[W_KING] = 1; expectCount[W_PAWN] = 8;
	expectCount[B_ROOK] = 2; expectCount[B_KNIGHT] = 2; expectCount[B_BISHOP] = 2;
	expectCount[B_QUEEN] = 1; expectCount[B_KING] = 1; expectCount[B_PAWN] = 8;
	int total = 0;
	for (Piece i = W_PAWN; i < PIECE_NB; ++i)
	{
		if (!(i == 8 || i == 7))
		{
			EXPECT_EQ(expectCount[i], board.count(i));
			total += expectCount[i];
		}
	}
	EXPECT_EQ(total, board.count(WHITE) + board.count(BLACK));

}

TEST_F(RepresentationTests, InitiateBoard_CheckBoardAreInSync)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	EXPECT_TRUE(board.is_ok());
}

TEST_F(RepresentationTests, BOARD_TO_FEN_ACCURATE)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq a3 0 1");
	EXPECT_EQ("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq a3 0 1", board.boardToFEN());
}