using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessUI.Model.UCI
{
    abstract class BaseCommand
    {
        public BaseCommand(string command)
        {
            Command = command;
        }

        public string Command { get;}

        public abstract void noteResponse(string str);

        public abstract string getReponse();

        /// <summary>
        /// Done is called when the response has been completly noted
        /// the child class call's it's own done event, which raises custom events
        /// *Note for this to be fired UCI needs to go call it -> MUST BE MAIN THREAD!!! 
        /// else it becomes unsafe and UCI thread interacts with code it must not
        /// </summary>
        public abstract void done();
    }
}
