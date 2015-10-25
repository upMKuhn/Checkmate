using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Threading;
using System.Windows;
using ChessUI.ViewModel;
using System.Diagnostics;
using System.IO;
namespace ChessUI
{
    /// <summary>
    /// Interaktionslogik für "App.xaml"
    /// </summary>
    public partial class App : Application
    {
        public const bool DEBUG = true;
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
            MainWindow window = new MainWindow();
            var viewModel = new MainViewModel();
            //Give the window the context
            window.DataContext = viewModel;
            window.Show();

            Thread console = new Thread(testConsole);
            console.Start();
        }


        private void testConsole()
        {
            ProcessStartInfo psi = new ProcessStartInfo( @"C:\Users\martin_kuhn\Documents\Visual Studio 2015\Projects\Checkmate\Build\UnitTests\x64\engine.exe");
            psi.UseShellExecute = false;
            psi.ErrorDialog = false;
            psi.RedirectStandardError = true;
            psi.RedirectStandardInput = true;
            psi.RedirectStandardOutput = true;
            psi.CreateNoWindow = true;

            Process process = new Process();
            process.StartInfo = psi;
            bool started = process.Start();

            StreamWriter stw = process.StandardInput;
            StreamReader str = process.StandardOutput;
            StreamReader sterror = process.StandardError;

            string input = str.ReadLine();

            if (input == "Hello World")
            {
                MessageBox.Show(input);
                stw.WriteLine("Hello Back!");
                stw.Flush();
            }
            
            //stw.WriteLine("I am GUI!");
            //stw.Flush();

        }

    }

}
