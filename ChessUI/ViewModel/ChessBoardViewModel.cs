using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ChessUI.DataAccess;
using System.Collections.ObjectModel;
using System.Windows.Input;
using ChessUI.Model;

namespace ChessUI.ViewModel
{
    public class ChessBoardViewModel : ViewModelBase 
    {
        private ChessBoardRepository _ChessBoardRepository;
        private ObservableCollection<SquareViewModel> _boardView;
        private Board myBoard;

        public ChessBoardViewModel(ChessBoardRepository chessBoardRepository)
        {
            if (chessBoardRepository == null)
            {
                throw new ArgumentNullException("chessBoardRepository");
            }

            this._ChessBoardRepository = chessBoardRepository;
            AllSquares = SquareViewModel.
                createSquareViewModelsList(chessBoardRepository.GetSquares(),this);
            myBoard = new Board(_ChessBoardRepository.GetSquares());
        }

        public ObservableCollection<SquareViewModel> AllSquares
        {   get;
            private set;
        }

        public ObservableCollection<PieceViewModel> AllPieces
        {
            get;
            private set;
        }

        public SquareViewModel findSquare(string pos)
        {
            foreach (SquareViewModel sqr in AllSquares)
            {
                if (sqr.Pos.SquareName == pos) { return sqr; }
            }
            return null;
        }
        
        //Move from square with null piece 
        // to delete pieces 
        public void makeMove(string from, string to)
        {
            SquareViewModel fromSqr = findSquare(from);
            SquareViewModel toSqr = findSquare(to);

            myBoard.movePiece(fromSqr.mySquare.Position, toSqr.mySquare.Position);
            toSqr.Piece = fromSqr.Piece;
            fromSqr.Piece = null;
            fromSqr.resetBorderBrush();
            toSqr.resetBorderBrush();
        }
        
        private ICommand _MovePieceCommand;
        public ICommand MovePieceCommand
        {
            get
            {
                if (_MovePieceCommand == null)
                {
                    _MovePieceCommand = new RelayCommand(param => this.MovePieceCommandExecute(param.ToString()), param => this.MovePieceCommandCanExecute(param.ToString()));
                }
                return _MovePieceCommand;
            }
        }

        void MovePieceCommandExecute(string squareSelected)
        {
            foreach (SquareViewModel sqr in AllSquares)
            {
                //Is correct Square + has piece 
                if (sqr.SelectPieceCommand.CanExecute(squareSelected))
                {

                    if (sqr.SelectionState == SquareViewModel.SelectStates.NotSelected)
                    {
                        sqr.SelectPieceCommand.Execute(squareSelected);
                    }
                    else if(sqr.Pos.SquareName == squareSelected)
                    {
                        sqr.resetBorderBrush();
                    }//if found a selected piece
                    else if (sqr.SelectionState == SquareViewModel.SelectStates.PieceSelected)
                    {
                        makeMove(sqr.Pos.SquareName, squareSelected);
                        break;
                    }
                }
            }
        }
        bool MovePieceCommandCanExecute(string squareClicked)
        {
            return true;
        }
    }
}
