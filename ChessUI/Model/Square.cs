using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ChessUI.Model
{
    public class Square
    {
        private Piece _piece;

        /// <summary>
        /// Important for coloring... I don't want to
        /// violate MVVM so I keep the color out of the model
        /// </summary>
        /// <param name="position"></param>
        /// <returns></returns>
        public static Square createSquare(Position position)
        {
            return new Square { Position = position };
        }
        public static Square createSquare(Position position,Piece piece)
        {
            return new Square { Position = position, Piece = piece };
        }


        public Position Position { get; set; }
        public Piece Piece
        { get { return _piece; } set { _piece = value; if (value != null) { value.mySquare = this; } } }
    }
}
