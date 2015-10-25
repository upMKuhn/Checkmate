using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;
using System.IO;

namespace ChessUI.Model.UCI
{
    class UCI
    {
        

        public UCI(string path)
        {
            this.m_psi = new ProcessStartInfo(@"C:\Users\martin_kuhn\Documents\Visual Studio 2015\Projects\Checkmate\Build\UnitTests\x64\engine.exe");
            thread = new Thread(start);
            m_app = new Process();
            m_cmdsToExecute = new Queue<UCI_Command>();
            m_reponse = new Queue<UCI_Command>();
            m_app.StartInfo = m_psi;
        }

        private void init()
        {
            m_psi.UseShellExecute = false;
            m_psi.ErrorDialog = false;
            m_psi.RedirectStandardError = true;
            m_psi.RedirectStandardInput = true;
            m_psi.RedirectStandardOutput = true;
            m_psi.CreateNoWindow = true;
            if (m_app.Start())
            {
                thread.Start();
            }
            else
            {
                throw new Exception("Engine did not start");
            }
        }

        private void start()
        {
            stw = m_app.StandardInput;
            str = m_app.StandardOutput;
            sterror = m_app.StandardError;

            bool isready = true;
            while (true)
            {
                BaseCommand cmd = FetchCommand();

                if (cmd.Command != "" && isready)
                {
                    stw.WriteLine(cmd.Command);
                    stw.Flush();
                }else if (!isready)
                {

                }


                Thread.Sleep(50);
                
            }
        }


        public Thread thread { get; }
        public event FEN_Update FEN;
        public delegate void FEN_Update(string FEN);

        public void Abort()
        {
            thread.Abort();
            m_app.Close();
        }

        private Queue<BaseCommand> m_cmdsToExecute;
        private Queue<BaseCommand> m_reponse;

        private void QueueCommand(BaseCommand cmd)
        {
            lock (thread)
            {
                m_cmdsToExecute.Enqueue(cmd);
            }
        }

        private BaseCommand FetchCommand()
        {
            BaseCommand cmd = null;
            lock(thread)
            {
                if (m_cmdsToExecute.Count > 0)
                {
                    cmd = m_cmdsToExecute.Dequeue();
                }
            }
            return cmd;
        }

        private Process m_app;
        private ProcessStartInfo m_psi;

        //can't be bother to type m_stw for such a heavily used var
        StreamWriter stw;
        StreamReader str;
        StreamReader sterror;

    }
}
