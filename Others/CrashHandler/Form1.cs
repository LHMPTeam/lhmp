using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Net;
using System.Text;
using System.Windows.Forms;

namespace CrashHandler
{
    public partial class Form1 : Form
    {
        string data = "";
        string nick = "Unknown";

        static string ReadAllText(string path)
        {
            using (var r = new StreamReader(path))
            {
                return r.ReadToEnd();
            }
        }

        public Form1(string input)
        {
            data = input;
            try
            {
                nick = ReadAllText(@"lhmp\nickname.txt");
            }
            catch
            {
                nick = "Unknown";
            }
            
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = nick;
            
            textBox1.ReadOnly = true;

            label5.Text += "0x"+data.Split('|')[0] + " at Module: " + data.Split('|')[2] + "\n" + ("EAX: " + data.Split('|')[1].Split('@')[0] + " ECX: " + data.Split('|')[1].Split('@')[1]
                 + " EDX: " + data.Split('|')[1].Split('@')[2]
                  + " EBX: " + data.Split('|')[1].Split('@')[3] + "\n"
                   + " ESP: " + data.Split('|')[1].Split('@')[4]
                    + "EBP: " + data.Split('|')[1].Split('@')[5]
                     + " ESI: " + data.Split('|')[1].Split('@')[6]
                      + " EDI: " + data.Split('|')[1].Split('@')[7]);
            textBox2.Focus();
            this.Text = "LH:MP Crash Sender";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.ExitThread();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string popis = "", rep = "";

            popis = textBox2.Text.Replace("|", "").Replace("#", "");
            rep = textBox3.Text.Replace("|", "").Replace("#", "");

            string send = data + "|" + nick + "|" + popis + "|" + rep;
            send = send.Replace(' ', '+');
            //MessageBox.Show(send);
            using (var wb = new WebClient())
            {
                var d = new NameValueCollection();
                d["data"] = send;
               // ASCIIEncoding a = new ASCIIEncoding();
                var response = wb.UploadValues(new Uri("http://www.lh-mp.eu/query/crash.php"), "POST", d);
                //MessageBox.Show(a.GetString((byte[])response.GetValue(0),0,response.Length));
                panel1.Visible = true;
                timer1.Start();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Application.ExitThread();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }
    }
}
