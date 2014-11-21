using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace CrashHandler
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] input)
        {
            //input = new string[1];
            //input[0] = "00700000|0@0@0@0@0@0@0@0|client.dll|03c7c0ace395am80182db07ae2c30f034";
            try { if (input[0] == "") { return; } else if(!input[0].Contains("|") && !input[0].Contains("@")){return;}}
            catch { return; }
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1(input[0]));
        }
    }
}
