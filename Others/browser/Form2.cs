using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace browser
{
    public partial class Form2 : Form
    {
        string[] nowdata;
        string build;
        public Form2(string[] data)
        {
            nowdata = data;
            build = data[0] + ":";
            build = build + data[1] + ":";
            
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                build = build + textBox1.Text;
                System.IO.File.AppendAllText("lhmp/favorite.txt", build + Environment.NewLine);
                this.Close();
            }
        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }
    }
}
