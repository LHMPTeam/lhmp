using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Autoupdater
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] pass)
        {
            try { if (pass[0] == "") { } }
            catch { pass = new string[1]; pass[0] = " "; }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1(pass[0]));
        }
    }
}
