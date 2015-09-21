using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ChessUI.Model;
using System.Windows.Input;
using AttachedCommandBehavior;
namespace ChessUI.ViewModel
{
    public class PieceViewModel
    {

        public static PieceViewModel create(Piece piece, ChessBoardViewModel parent)
        {
            if (piece == null)
            { return null; }
            return new PieceViewModel(piece, parent);
        }

        private string imagePath = "/ChessUI;component/Resources/";
        

        public PieceViewModel(Piece piece, ChessBoardViewModel parent)
        {
            MyPiece = piece;
            ParentViewModel = parent;
        }

        #region PROPERTIES
        public Piece MyPiece { get; private set; }
        public ChessBoardViewModel ParentViewModel { get; private set; }
        public PieceType PieceType {
                get { return MyPiece.myPieceType; }
                set { MyPiece.myPieceType = value; } 
            }
            public Player Color {
                get { return MyPiece.Color; }
                set { MyPiece.Color = value; }
            }

            public string PieceImageSource {
                get {
                    switch (PieceType)
                    {
                        case PieceType.Pawn:
                            if (Color == Player.WHITE)
                            {
                                return imagePath+"WHITEpawn.png";
                            }
                            else if (Color == Player.BLACK) {
                                return imagePath+"BLACKpawn.png";
                            }
                            break;

                        case PieceType.Rook:
                            if (Color == Player.WHITE)
                            {
                                return imagePath + "WHITERook.png";
                            }
                            else if (Color == Player.BLACK)
                            {
                                return imagePath + "BLACKRook.png";
                            }
                            break;
                        case PieceType.Knight:
                            if (Color == Player.WHITE)
                            {
                                return imagePath + "WHITEKnight.png";
                            }
                            else if (Color == Player.BLACK)
                            {
                                return imagePath + "BLACKKnight.png";
                            }
                            break;
                        case PieceType.Bishop:
                            if (Color == Player.WHITE)
                            {
                                return imagePath + "WHITEBishop.png";
                            }
                            else if (Color == Player.BLACK)
                            {
                                return imagePath + "BLACKBishop.png";
                            }
                            break;
                        case PieceType.Queen:
                            if (Color == Player.WHITE)
                            {
                                return imagePath + "WHITEQueen.png";
                            }
                            else if (Color == Player.BLACK)
                            {
                                return imagePath + "BLACKQueen.png";
                            }
                            break;
                        case PieceType.King:
                            if (Color == Player.WHITE)
                            {
                                return imagePath + "WHITEKing.png";
                            }
                            else if (Color == Player.BLACK)
                            {
                                return imagePath + "BLACKKing.png";
                            }
                            break;
                    }
                    return imagePath + "null.png";
                }
            }
        #endregion

        




    }
}
