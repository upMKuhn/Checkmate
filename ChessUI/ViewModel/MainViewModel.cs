
using ChessUI.DataAccess;
using System.Collections.ObjectModel;

namespace ChessUI.ViewModel
{
    public class MainViewModel : ViewModelBase
    {
        readonly ChessBoardRepository _ChessBoardRepository;

        ObservableCollection<ViewModelBase> _viewModels;



        public MainViewModel()
        {
            string startFEN = "rnbqkbnr/P7/8/5PpP/8/4p3/4pP1P/RNBQK2R w KQkq g5 0 1";
            _ChessBoardRepository = new ChessBoardRepository(startFEN);
            //Create an instance of our ViewModel and add it to
            //Collection
            ViewModelBase viewModel = new ChessBoardViewModel(_ChessBoardRepository);
            this.ViewModels.Add(viewModel);
        }

        public ChessBoardViewModel ChessBoard { get;private set; }
        public ObservableCollection<ViewModelBase> ViewModels
        {
            get
            {
                if (_viewModels == null)
                {
                    _viewModels = new ObservableCollection<ViewModelBase>();
                }
                return _viewModels;
            }
        }
    }
}
