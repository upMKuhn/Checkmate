using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ChessUI.DataAccess.UCI;
using ChessUI.DataAccess.Engine;
using ChessUI.DataAccess;
using ChessUI.Common;

namespace ChessUI.Model
{
    public class Board
    {
        private List<Square> _board;
        private Stack<Move> _moveHistory;

        public event EventHandler<MoveEventArg> MoveMade;
        public event EventHandler<PieceEventArg> PieceAdded;
        public event EventHandler<PieceEventArg> PieceRemoved;

        public Board(List<Square> board)
        {
            _board = board;
            _moveHistory = new Stack<Move>();
            MoveMade += OnMoveMade;
            SideToMove = Player.WHITE;
        }

        public Stack<Move> MoveHistory { get { return _moveHistory; } }

        public Player SideToMove { get; private set; }
        public Square getSquare(int index)
        {
            return _board[index];
        }
        public Piece getPiece(int index )
        {
            return _board[index].Piece;
        }
        public void PutPiece(int index, Piece p)
        {
            _board[index].Piece = p;
            p.mySquare = _board[index];
            RaisePieceAdded(p);
        }
        public void RemovePiece(int index)
        {
            Piece p = _board[index].Piece;
            _board[index].Piece = null;
            if(p != null)
                RaisePieceRemoved(p);
        }

        public IEnumerable<Piece> GetAllPieces()
        {
            List<Piece> pieces = new List<Piece>();

            foreach (Square sq in _board)
                if (sq.Piece != null)
                    pieces.Add(sq.Piece);
            return pieces;
        }


        public void movePiece(Position from, Position  to)
        {

            Square fromSqr = _board[from.Index];
            Square toSqr = _board[to.Index];
            if (!fromSqr.Piece.Equals(null))
            {
                Move newMv = new Move(from.Index, to.Index, fromSqr.Piece, toSqr.Piece);
                SideToMove = fromSqr.Piece.Color == Player.WHITE ? Player.BLACK : Player.WHITE;
                toSqr.Piece = fromSqr.Piece;
                fromSqr.Piece = null;
                _moveHistory.Push(newMv);
                RaiseMoveMade(newMv);
            }
            else { throw new Exception("From Square was null!"); }
        }

        public void UndoLastMove()
        {
            if (MoveHistory.Count > 0)
            {
                Move m = MoveHistory.Pop();
                Square fromSqr = _board[m.from];
                Square toSqr = _board[m.to];
                fromSqr.Piece = m.MovingPiece;
                toSqr.Piece = m.capture;
                RaiseMoveMade(m);
            }
        }

        public void Evaluate(EventHandler<CommandEventArgs> Callback = null)
        {
            Evaluate cmd = new Evaluate();
            cmd.Success += OnEvaluateSuccessfull;
            cmd.Success += Callback;
            SetBoardOnEngine();
            AsyncCommandRelayQueue.GetInstance().QueueCommand(cmd);
        }

        public void SetBoardOnEngine()
        {
            var parser = new FENParser(this);
            SetPosition cmd = new SetPosition(parser.FEN);
            AsyncCommandRelayQueue.GetInstance().QueueCommand(cmd);
        }

        public void FenToBoard(string fen)
        {
            FENParser bp = new FENParser(fen);
            clearAllPieces();
            foreach (Piece p in bp.PiecePlacement)
                PutPiece(p.mySquare.Position.Index, p);
        }

        private void clearAllPieces()
        {
            foreach (Square s in _board)
                RemovePiece(s.Position.Index);
        }

        private void RaisePieceAdded(Piece p)
        {
            if (PieceAdded != null)
                PieceAdded(this,new PieceEventArg{piece = p});
        }

        private void RaisePieceRemoved(Piece p)
        {
            if (PieceRemoved != null)
                PieceRemoved(this, new PieceEventArg { piece = p });
        }

        private void RaiseMoveMade(Move m)
        {
            if (MoveMade != null)
                MoveMade(this,new MoveEventArg{moveMade = m});
        }

        private void OnMoveMade(object sender, MoveEventArg move)
        {
            Evaluate();
        }

        private void OnEvaluateSuccessfull(Object sender, CommandEventArgs arg)
        {
            ResponseLine response = arg.Response.First<ResponseLine>();
            IChannel inputChannel = ChannelRepository.GetInputchannel();
            inputChannel.WriteLine("Evaluation Score: {0}", (int) response.TryGetValue("score"));
        }

    }

    public class PieceEventArg : EventArgs
    {
        public Piece piece { get; set; }
    }

    public class MoveEventArg : EventArgs
    {
        public Move moveMade { get; set; }
    }

}
