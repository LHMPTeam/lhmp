using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Net;
using System.Security.Cryptography;

namespace browser
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        bool bMouseDown = false;
        private Point firstPoint;

        private void button1_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.card = comboBox1.SelectedIndex;
            Properties.Settings.Default.nick = textBox3.Text;

            Properties.Settings.Default.Save();
            if (!File.Exists("lhmp/nickname.txt"))
            {
                File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            }
            else
            {
                File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            }
            Application.ExitThread();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            firstPoint = e.Location;
            bMouseDown = true;
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (bMouseDown)
            {
                // Get the difference between the two points
                int xDiff = firstPoint.X - e.Location.X;
                int yDiff = firstPoint.Y - e.Location.Y;

                // Set the new point
                int x = this.Location.X - xDiff;
                int y = this.Location.Y - yDiff;
                this.Location = new Point(x, y);
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            bMouseDown = false;
        }


        string serverip;
        string port;



        private void button5_Click(object sender, EventArgs e)
        {
            

            if (textBox1.Text != "" && textBox2.Text != "")
            {
                // play game
                try
                {
                    serverip = textBox1.Text;
                    port = textBox2.Text;
                    Properties.Settings.Default.serverip = textBox1.Text;
                    Properties.Settings.Default.port = textBox2.Text;
                    Properties.Settings.Default.Save();
                    PlayGame();
                }
                catch { MessageBox.Show("Invalid user input!"); textBox1.Text = ""; textBox2.Text = ""; }
            }
            else MessageBox.Show("IP Address and Server Port not specified!");
            if (textBox1.Text == "")
            {
                MessageBox.Show("IP Address not specified!");
        
            }
            else if (textBox2.Text == "") MessageBox.Show("Server Port not specified!");


        }

        void PlayGame()
        { 
            if (!File.Exists("lhmp/update.tmp") && File.ReadAllLines("lhmp\\browser_settings.txt")[1] == "0")
            {
                MessageBox.Show("Updater is checking file integrity... Please try again in a few seconds.");
                return;
            }
            else
            {
                File.Delete("lhmp/update.tmp");
            }
            if(!File.Exists("lhmp/client.dll"))
            {
                MessageBox.Show("File client.dll was not found. Reinstall LH-MP or Game");
                Application.ExitThread();
            }
            //Updatex();
            int a = 0;
            try
            {

                if(textBox3.Text=="")
                {
                    MessageBox.Show("Username not set. Use settings panel!");
                    a = 1;
                }
                if (a == 0)
                {
                    Updatex();

				            if (!File.Exists("lhmp/nickname.txt"))
							{
								File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
							}
							else
							{
								File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
							}
                    if (serverip.ToCharArray()[1] == '.' || serverip.ToCharArray()[2] == '.' || serverip.ToCharArray()[3] == '.') { }
                    else {
                        serverip = Dns.GetHostAddresses(serverip)[0].ToString();
                    }
                    Process.Start("loader.exe", serverip + " " + port);
                    Application.ExitThread();
                }
                else tabControl1.SelectedIndex = 2;
            }
            catch
            {
                MessageBox.Show("Program loader.exe was not found. Please reinstall LH-MP or Game Data!");
                Application.ExitThread();
            }
        }
        bool showBan = true;

        void Loadx()
        {
            
            try
            {
                textBox3.Text = File.ReadAllLines("lhmp\\browser_settings.txt")[0];
                comboBox2.SelectedIndex = Convert.ToInt32(File.ReadAllLines("lhmp\\browser_settings.txt")[1]);
                comboBox1.SelectedIndex = Convert.ToInt32(File.ReadAllLines("lhmp\\browser_settings.txt")[2]);
                comboBox3.SelectedIndex = Convert.ToInt32(File.ReadAllLines("lhmp\\browser_settings.txt")[3]);
            }
            catch { File.WriteAllText("lhmp\\browser_settings.txt", "" + "\n" + "0" + "\n" + "0" + "\n" + "0"); }
        }
        void Updatex()
        {
            //string numericBan = (showBan == true) ? "1" : "0";
            File.WriteAllText("lhmp\\browser_settings.txt", textBox3.Text + "\n" + comboBox2.SelectedIndex + "\n" + comboBox1.SelectedIndex + "\n" + comboBox3.SelectedIndex);

            Properties.Settings.Default.card = comboBox1.SelectedIndex;
            Properties.Settings.Default.nick = textBox3.Text;

            Properties.Settings.Default.Save();
            
            File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            //MessageBox.Show("save");
            Properties.Settings.Default.nick = textBox3.Text;
            Properties.Settings.Default.ifupdate = comboBox2.SelectedIndex;
            Properties.Settings.Default.card = comboBox1.SelectedIndex;
            Properties.Settings.Default.channel = comboBox3.SelectedIndex;
            Properties.Settings.Default.Save();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            tabControl1.SelectedIndex = 0;

            if (!File.Exists("Game.exe"))
            {
                MessageBox.Show("File Game.exe was not found. Reinstall LH-MP or Game");
                Application.ExitThread();
                return;
            }

            //Net-check
            try { WebClient request = new WebClient(); string r = request.DownloadString(new Uri("http://lh-mp.eu/alive")); if (r != "peer") { MessageBox.Show("Our main servers are offline! Please try again later or contact nearest support."); Application.ExitThread(); } }
            catch { MessageBox.Show("Our main servers are offline! Please try again later or contact nearest support."); Application.ExitThread(); }
            Process pr;
            if (!File.Exists("lh_updater.exe"))
            {
                MessageBox.Show("In order to use LH:MP, browser must download our updater. Your LH:MP will be installed then.");
                WebClient wb = new WebClient();
                try { wb.DownloadFile(new Uri("http://lh-mp.eu/_updater/lh_updater.exe"), "lh_updater.exe"); }
                catch { 
                    Application.ExitThread();
                }
                
                if(Properties.Settings.Default.channel == 1)
                    pr = Process.Start("lh_updater.exe","dev");
                else
                    pr = Process.Start("lh_updater.exe");
                //pr.WaitForExit();
            }
            else
            {
                WebClient request = new WebClient();
                string r = "";
                try { r = request.DownloadString("http://www.lh-mp.eu/_updater/ver.php?file=../lh_updater.exe"); }
                catch
                {
                    Application.ExitThread();
                }
                //MessageBox.Show(remote.ToString()); !Ide

                var md5 = MD5.Create();
                var stream = File.OpenRead("lh_updater.exe");
                string l = BitConverter.ToString(md5.ComputeHash(stream)).Replace("-", "").ToLower();
                stream.Close();
                
                if (l != r)
                {
                    MessageBox.Show("New version of updater is available. Downloading...");
                    File.Delete("lh_updater.exe");
                    try { request.DownloadFile(new Uri("http://lh-mp.eu/_updater/lh_updater.exe"), "lh_updater.exe"); }
                    catch { MessageBox.Show("Our main servers are offline! Please try again later or contact nearest support."); }
                   // if (Properties.Settings.Default.channel == 1)
                     //   pr = Process.Start("lh_updater.exe", "dev");
                    //else
                      //  pr = Process.Start("lh_updater.exe");
                    //pr.WaitForExit();
                }
            }
            zoznam.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            zoznam.MultiSelect = false;
            zoznam.RowPrePaint += new DataGridViewRowPrePaintEventHandler(dgv_RowPrePaint);
            Get();
            if (!Directory.Exists("lhmp"))
            {
                Directory.CreateDirectory("lhmp");
            }

            if (!File.Exists("lhmp\\browser_settings.txt"))
            {
                comboBox1.SelectedIndex = 0;
                comboBox2.SelectedIndex = 0;
                comboBox3.SelectedIndex = 0;
                tabControl1.SelectedIndex = 0;
                File.WriteAllText("lhmp\\browser_settings.txt", "" + "\n" + "0" + "\n" + "0" + "\n" + "0");
            }
           
//            setMOTD();    
            if (Properties.Settings.Default.serverip != "")
                textBox1.Text = Properties.Settings.Default.serverip;

            comboBox1.SelectedIndex = Properties.Settings.Default.card;

            
            if (Properties.Settings.Default.port != "")
                textBox2.Text = Properties.Settings.Default.port;
            if (Properties.Settings.Default.nick != "")
                textBox3.Text = Properties.Settings.Default.nick;
            
            comboBox3.SelectedIndex = Properties.Settings.Default.channel;            
                tabControl1.SelectedIndex = Properties.Settings.Default.card;

                comboBox2.SelectedIndex = Properties.Settings.Default.ifupdate;

            if (!File.Exists("lhmp/favorite.txt"))
            {
                File.Create("lhmp/favorite.txt").Close();
            }
            else
            {
                string[] servers = File.ReadAllLines("lhmp/favorite.txt");

                for (int i = 0; i < servers.Length; i++)
                {
                    listBox1.Items.Add(servers[i]);
                }
            }
            if (!File.Exists("lhmp/nickname.txt"))
            {
                File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            }
            else
            {
                File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            }

            Loadx();

            try {
                if (comboBox2.SelectedIndex == 0)
                {
                    //MessageBox.Show("la");
                    if (comboBox3.SelectedIndex == 1)
                        pr = Process.Start("lh_updater.exe", "dev");
                    else
                        pr = Process.Start("lh_updater.exe");
                    
                    //pr.WaitForExit();
                }
            }
            catch{}


            /*
            if (showBan)
            {
                Form3 dlg = new Form3();
                dlg.ShowDialog();
                showBan = false;
                //MessageBox.Show("How?");
            } asi som zle uploadol Form3.cs, ajtak nemal daky extra ucel
             * */ 
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (File.Exists("lhmp/update.tmp"))
            {
                try
                {
                    File.Delete("lhmp/update.tmp");
                }
                catch { }
            }
            Updatex();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            tabControl1.SelectedIndex = 1;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Get();
            tabControl1.SelectedIndex = 0;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            tabControl1.SelectedIndex = 2;
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("http://lh-mp.eu/");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            if (!File.Exists("lhmp/favorite.txt"))
            {
                File.Create("lhmp/favorite.txt").Close();
            }
            else
            {
                string[] servers = File.ReadAllLines("lhmp/favorite.txt");

                for (int i = 0; i < servers.Length; i++)
                {
                    listBox1.Items.Add(servers[i]);
                }
            }
            tabControl1.SelectedIndex = 3;
        }

        private void button8_Click(object sender, EventArgs e)
        {
            try
            {
                string server = listBox1.SelectedItem.ToString();
                string[] splitter = server.Split(':');
                serverip = splitter[0];
                port = splitter[1];
                PlayGame();
            }
            catch { MessageBox.Show("Select server first!"); }
        }

        private void button7_Click(object sender, EventArgs e)
        {
            try
            {
                File.Delete("lhmp/favorite.txt");
                listBox1.Items.RemoveAt(listBox1.SelectedIndex);
                int count = listBox1.Items.Count;
                string[] serverobnov = new string[count];
                for (int i = 0; i < listBox1.Items.Count; i++)
                {
                    serverobnov[i] = listBox1.Items[i].ToString();
                }
                File.WriteAllLines("lhmp/favorite.txt", serverobnov);
            }
            catch { MessageBox.Show("Select server first!"); }
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "" && textBox2.Text != "")
            {
                string[] data = { textBox1.Text, textBox2.Text };
                Form2 dlg = new Form2(data);
                dlg.ShowDialog();
            }
        }
      

        private void pictureBox2_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {
       
        }

        private void label6_MouseHover(object sender, EventArgs e)
        {

        }

        private void label6_MouseMove(object sender, MouseEventArgs e)
        {
            
        }

        private void label6_MouseEnter(object sender, EventArgs e)
        {
       
        }

        private void button10_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Don't forget to leave one blank line at the end of the file!");
            try { Process.Start("lhmp\\favorite.txt"); }
            catch { File.WriteAllText("lhmp\\favorite.txt", "\n"); try { Process.Start("lhmp\\favorite.txt"); } catch { Directory.CreateDirectory("lhmp"); File.WriteAllText("lhmp/favorite.txt", "\n"); try { Process.Start("lhmp/favorite.txt"); } catch { MessageBox.Show("Weird error occured. Be sure that browser has privileges for creating and editing folders and files, also run browser as admin. If the problem persists, try to add one server via browser or create file favorite.txt in folder lhmp manually."); } } }
        }

        private void serverlist_Tick(object sender, EventArgs e)
        {
          
        }
        string[] ipdata;

        void Get()
        {
            zoznam.Rows.Clear();
            //servers.Items.Clear();
            using (WebClient client = new WebClient())
            {
                Application.DoEvents();
                string htmlCode = "";
                try { htmlCode = client.DownloadString("http://lh-mp.eu/query/get.php"); } 
                catch { MessageBox.Show("Our main servers are offline! Please try again later or contact nearest support."); }
                try{
                string[] lines = htmlCode.Split('\n');
                ipdata = new string[lines.Length];
                for(int i = 0; i < lines.Length-1; i++){
                    string[] data = lines[i].Split('%');
                    ipdata[i] = data[1] + "|" + data[2];
              //      string show = data[0] + "               " + data[1] + ":" + data[2] + "                 " + data[3] + "/" + data[4] + "                 " + data[5];
                //    servers.Items.Add(show);
                    zoznam.Rows.Add(data[0], data[1] + ":" + data[2], data[3] + "/" + data[4], data[5]);
                    
                }
                }
                catch{
                    
                }
                
                
            }

        }
        private void servers_DrawItem(object sender, DrawItemEventArgs e)
        {
            e.DrawBackground();
            Graphics g = e.Graphics;
            Brush brush = ((e.State & DrawItemState.Selected) == DrawItemState.Selected) ?
                          Brushes.DarkRed : new SolidBrush(e.BackColor);
            g.FillRectangle(brush, e.Bounds);
            try
            {
                e.Graphics.DrawString(servers.Items[e.Index].ToString(), e.Font,
                         new SolidBrush(e.ForeColor), e.Bounds, StringFormat.GenericDefault);
                e.DrawFocusRectangle();
            }
            catch { }
        }

        private void servers_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void servers_DoubleClick(object sender, EventArgs e)
        {
            try
            {
                int i = servers.SelectedIndex;
                string[] IP = ipdata[i].Split('|');
                serverip = IP[0];
                port = IP[1];
                PlayGame();
            }
            catch { }
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void zoznam_Paint(object sender, PaintEventArgs e)
        {
            
        }

        private void zoznam_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {

        }

        private void zoznam_DoubleClick(object sender, EventArgs e)
        {
            try
            {
                int i = zoznam.CurrentRow.Index;
                string[] IP = ipdata[i].Split('|');
                serverip = IP[0];
                port = IP[1];
                PlayGame();
            }
            catch { }
        }
        private void dgv_RowPrePaint(object sender, DataGridViewRowPrePaintEventArgs e)
        {
            e.PaintParts &= ~DataGridViewPaintParts.Focus;
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            try
            {
                int i = zoznam.CurrentRow.Index;
                string[] IP = ipdata[i].Split('|');
                serverip = IP[0];
                port = IP[1];
                PlayGame();
            }
            catch { }
        }

        private void toolStripMenuItem2_Click(object sender, EventArgs e)
        {
            try
            {
                int i = zoznam.CurrentRow.Index;
                string[] IP = ipdata[i].Split('|');
                string build = IP[0] + ":" + IP[1] + ":" + zoznam.CurrentRow.Cells[0].Value;
                System.IO.File.AppendAllText("lhmp/favorite.txt", build + Environment.NewLine);
            }
            catch { }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.card = comboBox1.SelectedIndex;
            Properties.Settings.Default.nick = textBox3.Text;

            Properties.Settings.Default.Save();
            if (!File.Exists("lhmp/nickname.txt"))
            {
                File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            }
            else
            {
                File.WriteAllText("lhmp/nickname.txt", textBox3.Text);
            }
            label10.Show();
            done.Start();
        }

        private void done_Tick(object sender, EventArgs e)
        {
            label10.Hide();
            done.Stop();
        }

        private void label5_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.lh-mp.eu/");
        }

        private void label9_Click(object sender, EventArgs e)
        {
            Process.Start("http://www.lh-mp.eu/");
        }

        private void button12_Click(object sender, EventArgs e)
        {
            Updatex();
            try
            {
                Process pr;
                if (Properties.Settings.Default.channel == 1)
                    pr = Process.Start("lh_updater.exe", "dev");
                else
                    pr = Process.Start("lh_updater.exe");
                //pr.WaitForExit();
            }
            catch { MessageBox.Show("lh_updater.exe is missing. Please reinstall LH:MP or game data!"); }
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
  //          Updatex();
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
    //        Updatex();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button13_Click(object sender, EventArgs e)
        {
            Updatex();
            try
            {
                Process pr;
                
                pr = Process.Start("lh_updater.exe", "browser");
                Application.ExitThread();
                pr.WaitForExit();
            }
            catch { MessageBox.Show("lh_updater.exe is missing. Please reinstall LH:MP or game data!"); }
        }
    }
}
