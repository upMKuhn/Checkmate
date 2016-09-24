using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace ChessUI{
    public enum Player { NO_PLAYER,WHITE,BLACK}
    public enum PieceType {
        NO_PIECE,
        Pawn,
        Rook,
        Bishop,
        Knight,
        Queen,
        King
    }
}

namespace ChessUI.Model
{

    public class Piece
    {
        private Board _board;

        public Piece(Player color, PieceType pieceType, Square square,Board board)
        {
            init(color, pieceType, square, board);
        }

        public Piece(Player color, PieceType pieceType, Square square)
        {
            init(color, pieceType, square, null);
        }

        private void init(Player color, PieceType pieceType, Square square, Board board)
        {
            if (color == Player.NO_PLAYER)
            {
                throw new ArgumentNullException("Color NONE");
            }
            if (pieceType == PieceType.NO_PIECE)
            {
                throw new ArgumentNullException("pieceType NONE");
            }
            if (square == null)
            {
                throw new ArgumentNullException("square NULL");
            }

            this.Color = color;
            this.myPieceType = pieceType;
            this.mySquare = square;
            this.mySquare.Piece = this;
            _board = board;
        }

        public Player Color { get;  set; }
        public PieceType myPieceType { get; set; }
        public Board MyBoard { get { return _board; } }

        public Square mySquare { get; set; }

        public int GetFile()
        {
            return mySquare.Position.File;
        }

        public int GetRank()
        {
            return mySquare.Position.Rank;
        }

        public static bool operator== (Piece left, Piece right)
        {
            if (object.ReferenceEquals(left, null) && object.ReferenceEquals(right, null))
                return true;
            if (object.ReferenceEquals(left, null) || object.ReferenceEquals(right, null))
                return false;
            return left.myPieceType == right.myPieceType
                   && left.Color == right.Color;
        }

        public static bool operator!=(Piece left, Piece right)
        {
            if (object.ReferenceEquals(left, null) && object.ReferenceEquals(right, null))
                return false;
            if (object.ReferenceEquals(left, null) || object.ReferenceEquals(right, null))
                return true;
            return left.myPieceType != right.myPieceType
                   && left.Color != right.Color;
        }
        public static bool operator!=(Piece left, object right)
        { return !Object.Equals(left, right); }
        public static bool operator==(Piece left, object right)
        { return Object.Equals(left, right); }

        public override string ToString()
        {
            char c = '-';

            switch (myPieceType)
            {
                case PieceType.Pawn:
                    c = 'P';
                    break;
                case PieceType.Queen:
                    c = 'Q';
                    break;
                case PieceType.Rook:
                    c = 'R';
                    break;
                case PieceType.Bishop:
                    c = 'B';
                    break;
                case PieceType.King:
                    c = 'K';
                    break;
                case PieceType.Knight:
                    c = 'N';
                    break;
            }
            c = Color == Player.WHITE ? c : char.ToLower(c);
            return c.ToString();
        }

    }
}
