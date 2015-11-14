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

TEST_F(RepresentationTests, MoveRooksCheckCastelingRevoked)
{
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/8/RNBQKBNR w KQkq - 0 1";
	Represenation board(fen);
	Move mvQueenRook = make<NORMAL>(SQ_A1, SQ_A3);
	Move mvKingRook = make<NORMAL>(SQ_H1, SQ_H3);
	board.makeMove(mvQueenRook);
	EXPECT_EQ((BLACK_OO | BLACK_OOO | WHITE_OO), board.castlingRights);
	board.sideToMove = ~board.sideToMove;
	board.makeMove(mvKingRook);
	EXPECT_EQ((BLACK_OO | BLACK_OOO), board.castlingRights);
}

TEST_F(RepresentationTests, MovePawnCheckEnpassant)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move mv = make<NORMAL>(SQ_A2, SQ_A4);
	board.makeMove(mv);
	EXPECT_EQ(SQ_A4, board.enPassant);
}

TEST_F(RepresentationTests, MakeNormalMoveAndUndo)
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

TEST_F(RepresentationTests, MakeCaptureMoveAndUndo)
{
	string fen = "rnb1kbnr/pppppppp/8/8/q7/8/8/RNBQKBNR w KQkq - 0 1";
	Represenation board(fen);
	Move mv = make<NORMAL>(SQ_D1, SQ_A4, QUEEN);
	board.makeMove(mv);
	EXPECT_EQ(QUEEN, board.piece_on(SQ_A4)); //Prove FEN must have changed
	board.undoMove();
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MakeCastelingMoveAndUndo)
{
	string fen = "rnb1kbnr/pppppppp/8/8/4q3/8/8/R3K2R w KQkq - 0 1";
	Represenation board(fen);
	Move mv = make<CASTLING>(SQ_E1, SQ_A1);
	board.makeMove(mv);
	EXPECT_EQ(KING, board.piece_on(SQ_C1)); //Prove FEN must have changed
	EXPECT_EQ(ROOK, board.piece_on(SQ_D1)); //Prove FEN must have changed
	board.undoMove();
	EXPECT_EQ(fen, board.boardToFEN());
}

TEST_F(RepresentationTests, MoveTwo_UndoOne_CheckFen)
{
	string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	string expectFen = "rnbqkbnr/pppppppp/8/8/8/5N2/PPPPPPPP/RNBQKB1R b KQkq - 1 1";
	Represenation board(startFen);
	Move mv = make<NORMAL>(SQ_G1, SQ_F3, KNIGHT);
	Move mv2 = make<NORMAL>(SQ_G7, SQ_G5);
	board.makeMove(mv);
	board.makeMove(mv2);
	board.undoMove();

	EXPECT_EQ(expectFen , board.boardToFEN());
	EXPECT_EQ(B_PAWN, board.piece_on(SQ_G7));
}

TEST_F(RepresentationTests, INITATE_BOARD_CHECK_PIECE_COUNT)
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

TEST_F(RepresentationTests, BOARDS_ARE_IN_SYNC)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	EXPECT_TRUE(board.areAllBoardsOk());
}

TEST_F(RepresentationTests, BOARD_TO_FEN_ACCURATE)
{
	Represenation board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq a3 0 1");
	EXPECT_EQ("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq a3 0 1", board.boardToFEN());
}