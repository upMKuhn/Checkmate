using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessUI.Model
{
    public class Board
    {
        private List<Square> _board;
        private List<Move> _moveHistory;

        public Board(List<Square> board)
        {
            _board = board;
            _moveHistory = new List<Move>();
        }

        public List<Move> MoveHistory { get { return _moveHistory; } }

        public Square getSquare(int index)
        {
            return _board[index];
        }
        public Piece getPiece(int index )
        {
            return _board[index].Piece;
        }

        public void movePiece(Position from, Position  to)
        {
            Square fromSqr = _board[from.Index];
            Square toSqr = _board[to.Index];
            if (fromSqr.Piece != null)
            {
                Move newMv = new Move();
                newMv.Set(from.Index, to.Index, fromSqr.Piece, toSqr.Piece);
                toSqr.Piece = fromSqr.Piece;
                fromSqr.Piece = null;
                _moveHistory.Add(newMv);
            }
            else { throw new Exception("From Square was null!"); }
        }
    }
    public struct Move
    {
        public void Set(int from, int to, Piece MovingPiece, Piece capture)
        {
            this.from = from; this.to = to; this.MovingPiece = MovingPiece; this.capture = capture;
        }
        int from;
        int to;
        Piece MovingPiece;
        Piece capture;
       
    }
}
