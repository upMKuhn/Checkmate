using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ChessUI.Model;
using System.Windows;

namespace ChessUI.DataAccess
{

    public class ChessBoardRepository
    {
        readonly List<Square> _squareList;
        readonly List<Piece> _pieceList;
        private Board _board;
        public ChessBoardRepository(string FEN)
        {
            if (FEN == null) { throw new ArgumentNullException("FEN"); }
            _squareList = new List<Square>();
            _pieceList = new List<Piece>();
            //Fill 
            initSquareList();
            _board = new Board(_squareList);
            FEN_ToBoard(FEN);
        }


        public ChessBoardRepository()
        {
            _squareList = new List<Square>(64);
            initSquareList();
            _board = new Board(_squareList);
            
        }

        public Board getBoard()
        {
            return _board;
        }
        public List<Square> GetSquares() { return _squareList; }
        private Square getSquare(Position pos)
        { return _squareList[pos.Index]; }
        private void initSquareList()
        {
            Square[] board = new Square[64];
            for (int row = 8; row >= 1; row--)
            {
                for (int column = 8; column >= 1; column--)
                {   
                    Square sqr = Square.createSquare(new Position(row, column));
                    board[sqr.Position.Index] = sqr;

                }
            }
            _squareList.AddRange(board);
        }

        public void FEN_ToBoard(string FEN)
        {
            if (FEN == null) { throw new ArgumentNullException("FEN"); }
            _pieceList.Clear();

            int i = 0;

            int file = 8;
            int rank = 1;
            while (FEN[i] != ' ' || file < 1)
            {
                if (file < 1)
                {
                    throw new Exception("Board File over 8! R:" + file);
                }
                if (rank > 8)
                {
                    rank = 1;
                }

                Player color;
                if (Char.IsUpper(FEN[i]))
                {
                    color = Player.WHITE;
                }
                else
                {
                    color = Player.BLACK;
                }

                //Convert FEN char to position
                //and PieceType
                PieceType pieceType = PieceType.NO_PIECE;
                switch (char.ToLower(FEN[i]))
                {
                    case 'p':
                        pieceType = PieceType.Pawn;
                        break;
                    case 'r':
                        pieceType = PieceType.Rook;
                        break;
                    case 'n':
                        pieceType = PieceType.Knight;
                        break;
                    case 'b':
                        pieceType = PieceType.Bishop;
                        break;
                    case 'k':
                        pieceType = PieceType.King;
                        break;
                    case 'q':
                        pieceType = PieceType.Queen;
                        break;
                    case '/':
                        rank= 0;
                        file--;
                        break;
                    default:
                        if (char.IsDigit(FEN[i]))
                        {
                            int test = (int)Char.GetNumericValue(FEN,i);
                            rank = test+rank>8?1:rank+test;
                        }
                        break;
                }
                
                if (color != Player.NO_PLAYER && pieceType != PieceType.NO_PIECE)
                {
                    Square square = getSquare(new Position(file,rank));
                    _pieceList.Add(Piece.createPiece(color, pieceType, square,_board));
                }

                //next FEN CHAR
                rank++;
                i++;
            }
            i = 0;
        }

        
    }
}
