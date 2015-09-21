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

        public static Piece  createPiece(Player color, PieceType piece_Type, Square square,Board myBoard)
        {
            return new Piece(color, piece_Type, square,myBoard);
        }
        
        private Board _board;
        public Piece(Player color, PieceType pieceType, Square square,Board board)
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
        
    }
}
