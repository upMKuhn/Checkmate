using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Threading;
using System.Windows;
using System.IO;
using ChessUI.ViewModel;
using ChessUI.DataAccess.UCI;
using ChessUI.Model;
using System.IO;
using ChessUI.Common;
using ChessUI.DataAccess;

namespace ChessUI
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {
        public const bool DEBUG = false;

        public static bool IsWindowOpen<T>(string name = "") where T : Window
        {
            return string.IsNullOrEmpty(name)
               ? Application.Current.Windows.OfType<T>().Where(obj => obj.IsVisible == true).Any()
               : Application.Current.Windows.OfType<T>().Where(obj => obj.IsVisible == true).Any(w => w.Name.Equals(name));
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            StartMainWindow();
            PrepareEngine();
            base.OnStartup(e);
        }

        void StartMainWindow()
        {
            MainWindow window = new MainWindow();
            var viewModel = new MainViewModel();
            //Give the window the context
            window.DataContext = viewModel;
            window.Show();
        }

        void PrepareEngine()
        {
            UCIChannel com = new UCIChannel(Settings.ENGINE_PATH);
            ChannelRepository.RegisterOutput(com);
            com.Start();
        }

        void LaunchDebugWindow()
        {
                var win = new DebugWindow();
                win.DataContext = new ViewModel.debug.EngineInterfaceViewModel();
                win.Show();
        }



    }

}
