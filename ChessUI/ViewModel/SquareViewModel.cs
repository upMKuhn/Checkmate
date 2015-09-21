using ChessUI.Model;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;
using System.Windows.Media;
namespace ChessUI.ViewModel
{
    public class SquareViewModel : ViewModelBase
    {
        /// <summary>
        /// Factory methode. That produces bulk Square View Model
        /// </summary>
        /// <param name="squares"></param>
        /// <returns></returns>
        public static ObservableCollection<SquareViewModel> createSquareViewModelsList(List<Square> squares, ChessBoardViewModel parent)
        {
            ObservableCollection<SquareViewModel> squareBoard = new ObservableCollection<SquareViewModel>();
            foreach (Square sqr in squares)
            {
                squareBoard.Add(new SquareViewModel(sqr, parent));
                
            }
            //Reverse list to display the board correctly
            return new ObservableCollection<SquareViewModel>(squareBoard.Reverse<SquareViewModel>());
        }


        private ICommand _click_Select;
        private Brush _backgroundBrush;
        private Brush _borderBrush;
        private Piece _piece;
        private Color defaultBackground;
        
        public SquareViewModel(Square modelSquare, ChessBoardViewModel parent)
        {
            if (modelSquare == null)
            {
                throw new ArgumentNullException("Square");
            }
            mySquare = modelSquare;
            _piece = modelSquare.Piece ;
            ParentViewModel = parent;

            Color background = isSquareBlack() ? Colors.Brown : Colors.BurlyWood;
            _backgroundBrush = new SolidColorBrush(background);
            defaultBackground = background;
        }
        public SquareViewModel(Square modelSquare, Piece piece, ChessBoardViewModel parent)
        {
            if (modelSquare == null)
            {
                throw new ArgumentNullException("Square");
            }
            mySquare = modelSquare;
            _piece = piece;
            ParentViewModel = parent;

            Color background = isSquareBlack() ? Colors.Brown : Colors.BurlyWood;
            _backgroundBrush = new SolidColorBrush(background);
            defaultBackground = background;
        }

        #region Properties
            public enum SelectStates
            { NotSelected, PieceSelected, CanGoTo }
            public SelectStates SelectionState { get; set; }

            public ChessBoardViewModel ParentViewModel { get; private set; }
            public PieceViewModel Piece
            {   get { return PieceViewModel.create(_piece,ParentViewModel); }
                set { if (value != null) { _piece = value.MyPiece; } else { _piece = null; } OnPropertyChanged("Piece"); }
            }

            public Position Pos { get{ return this.mySquare.Position; } }

            public String SquareName
            {
                get
                {
                    
                     return mySquare.Position.SquareName; 
                    
                }
            }

            public Square mySquare { get; private set; }

            public Brush SquareColor
            { get
                {
                    return _backgroundBrush;
                }
                set
                {
                    _backgroundBrush = value;
                    OnPropertyChanged("SquareColor");
                }
            }
            public Brush SquareBorderBrush
            {
                get
                {
                    return _borderBrush == null ? SquareColor : _borderBrush;
                }
                set { _borderBrush = value; OnPropertyChanged("SquareBorderBrush"); }
            }
        #endregion

        #region Methodes

            /// <summary>
            /// Turn 2D Coordinates into 1D Index
            /// </summary>
            /// <param name="_pos"></param>
            /// <returns></returns>
            public int getIndex(Position _pos)
            {
                int value = (int)(8 * (_pos.File - 1) + _pos.Rank);
                return value;
            }
            public void resetBorderBrush()
            { SquareBorderBrush = new SolidColorBrush(defaultBackground); SelectionState = SelectStates.NotSelected; }

            public bool isSquareBlack()
            {
                return mySquare.Position.isSquareBlack;
            }
        #endregion

        #region Highlight Commands
            
        /// <summary>
        /// Select a square, piece is on that is intended to be
        /// moved.
        /// </summary>
        public ICommand SelectPieceCommand
        {
            get
            {
                if (_click_Select == null)
                {
                        
                    _click_Select = new RelayCommand(param => this.SelectPieceCommandExecute( param.ToString()), param => this.SelectPieceCommandCanExecute);
                }
                return _click_Select;
            }
        }
        private 
        void SelectPieceCommandExecute(string  clickedSquareName)
        {
            //Toggle selection + Correct sqr? else deselect
            if (SelectionState == SelectStates.NotSelected && clickedSquareName == this.SquareName)
            {
                SquareBorderBrush = new SolidColorBrush(Colors.DarkBlue);
                SelectionState = SelectStates.PieceSelected;
            }else
            {
                resetBorderBrush();
                SelectionState = SelectStates.NotSelected;
            }
        }
        bool SelectPieceCommandCanExecute
        {
            get
            {
                bool test = Piece != null;
                return test;
            }
        }
        #endregion
    }
}
