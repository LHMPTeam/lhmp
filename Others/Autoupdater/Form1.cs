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

namespace Autoupdater
{
    public partial class Form1 : Form
    {
        string pwd = "";
        public Form1(string pass)
        {
            pwd = pass;
            InitializeComponent();
        }
        String local;
        String remote;
        String[] files;
        bool next = false;
        String filename;
        int filec = 0;
        bool toupdate = false;
        private void Form1_Load(object sender, EventArgs e)
        {
            //MessageBox.Show(pwd);
            if (pwd == "dev")
            {
                //MessageBox.Show(lh_updater.Properties.Settings.Default.pass);
                WebClient request = new WebClient();
                string pass = request.DownloadString("http://www.lh-mp.eu/_updater/sewer.txt");
                if (lh_updater.Properties.Settings.Default.pass == pass) { Update("_dev"); return; }
                else panel1.Show();
            }
            else
            {
                Update();
            }
           
        }
        void Update(string mod="")
        {

            goto skok;
            if(File.Exists("browser.exe"))
            {
                WebClient requeste = new WebClient();
                string r = "";
                try { r = requeste.DownloadString("http://www.lh-mp.eu/_updater/ver.php?file=../browser.exe"); }
                catch
                {
                    Application.ExitThread();
                }
                //MessageBox.Show(remote.ToString()); !Ide

                foreach (Process proc in Process.GetProcessesByName("browser"))
                {
                    proc.Kill();
                }
                //MessageBox.Show("ok");
                var md5 = MD5.Create();
                var stream = File.OpenRead("browser.exe");
                string l = BitConverter.ToString(md5.ComputeHash(stream)).Replace("-", "").ToLower();
                stream.Close();

                if (l != r)
                {
                    if (r != lh_updater.Properties.Settings.Default.lasthash || mod=="browser")
                    {
                        DialogResult dialogx = MessageBox.Show("New version of browser is available. Do you want to download it? Press YES to download. Press NO to cancel and prompt when another version comes.", "Update is available", MessageBoxButtons.YesNo);
                        if (dialogx == System.Windows.Forms.DialogResult.Yes)
                        {
                            
                            Process.Start("http://www.lh-mp.eu/_updater/browser.exe");
                            
                        }
                        else
                        {
                            lh_updater.Properties.Settings.Default.lasthash = r;
                            lh_updater.Properties.Settings.Default.Save();
                        }

                    }
                }
                else
                {
                    Process.Start("browser.exe");
                }
                }







            if (mod == "browser") { Application.ExitThread(); }
            skok:



            if (!Directory.Exists("lhmp")) { Directory.CreateDirectory("lhmp"); }
            WebClient request = new WebClient();
            string filelist = "", deletelist = "";
            try
            {
                filelist = request.DownloadString("http://lh-mp.eu/_updater/filelist"+mod+".php");
                deletelist = request.DownloadString("http://lh-mp.eu/_updater/deletelist"+mod+".txt");
            }
            catch { MessageBox.Show("Our main servers are offline! Please try again later or contact nearest support."); }
            string[] delete = deletelist.Split('\n');
            foreach (string del in delete)
            {
                try
                {
                    if (File.Exists(del)) { File.Delete(del); }
                }
                catch { }
            }
            files = filelist.Split('\n');
            foreach (string file in files)
            {
                filename = file;
                try
                {
                    if (!file.Contains(".")) { try { Directory.CreateDirectory(file); } catch { } continue; }
                    try { remote = request.DownloadString("http://www.lh-mp.eu/_updater/ver"+mod+".php?file=" + file); }
                    catch
                    {
                        MessageBox.Show("Our main servers are offline! Please try again later or contact nearest support."); foreach (Process proc in Process.GetProcessesByName("lh_updater"))
                        {
                            proc.Kill();
                        }
                    }
                    //MessageBox.Show(remote.ToString()); !Ide

                    var md5 = MD5.Create();
                    var stream = File.OpenRead(file);
                    local = BitConverter.ToString(md5.ComputeHash(stream)).Replace("-", "").ToLower();
                    stream.Close();
                    if (local != remote)
                    {
                        // MessageBox.Show(file);
                        if (toupdate == false)
                        {
                            DialogResult dialog = MessageBox.Show("New update is available. Do you want to download it?", "Update is available", MessageBoxButtons.YesNo);
                            if (dialog == System.Windows.Forms.DialogResult.No)
                            {
                                try
                                {
                                    if (!File.Exists("lhmp\\update.tmp"))
                                    {
                                        File.WriteAllText("lhmp\\update.tmp", "1");
                                    }
                                    stream.Close();
                                    foreach (Process proc in Process.GetProcessesByName("lh_updater"))
                                    {
                                        proc.Kill();
                                    }
                                }
                                catch (Exception ex)
                                {
                                    //    MessageBox.Show(ex.Message);
                                }
                            }
                            toupdate = true;
                        }

                        // MessageBox.Show("LoL");
                        WebClient wb = new WebClient();

                        wb.DownloadProgressChanged += new DownloadProgressChangedEventHandler(wb_DownloadProgressChanged);
                        wb.DownloadFileCompleted += new AsyncCompletedEventHandler(wb_DownloadDataCompleted);
                        File.Delete(file);
                        wb.DownloadFileAsync(new Uri("http://www.lh-mp.eu/_updater/files"+mod+"/" + file), file);

                    }
                    else
                    {
                        continue;
                    }
                }
                catch (Exception ex)
                {
                    WebClient wb = new WebClient();

                    wb.DownloadProgressChanged += new DownloadProgressChangedEventHandler(wb_DownloadProgressChanged);
                    wb.DownloadFileCompleted += new AsyncCompletedEventHandler(wb_DownloadDataCompleted);
                    //File.Delete(@"lhmp\client.dll");
                    wb.DownloadFileAsync(new Uri("http://www.lh-mp.eu/_updater/files"+mod+"/" + file), file);
                }
                filename = file;
                filec++;
                while (!next)
                {
                    Application.DoEvents();
                }
                next = false;
                progressBar1.Value = 0;
            }
            if (filec != 0)
                MessageBox.Show("Update has been succesful.");
            if(!File.Exists("lhmp\\update.tmp")){
                File.WriteAllText("lhmp\\update.tmp", "1");
            }
            this.Close();
            try
            {
                foreach (Process proc in Process.GetProcessesByName("lh_updater"))
                {
                    proc.Kill();
                }
            }
            catch (Exception ex)
            {
                //    MessageBox.Show(ex.Message);
            }
            Application.ExitThread();
        }
        void wb_DownloadDataCompleted(object sender, AsyncCompletedEventArgs e)
        {
            //Spracuvanie
            Application.DoEvents();
            if (File.Exists(filename) || Directory.Exists(filename))
            {
                //Process up = Process.Start("update.exe");
                //up.WaitForExit();

                //File.Delete("update.exe");
                if (filec == files.Length) {  }
                next = true;
                
            }
            else
            {
                MessageBox.Show("Unknown Error. "+filename);
            }
        }

        void wb_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;
            Application.DoEvents();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
           
        }

        private void button1_Click(object sender, EventArgs e)
        {

            if (textBox1.Text == "") { return; }

            WebClient request = new WebClient();
            string pass = request.DownloadString("http://www.lh-mp.eu/_updater/sewer.txt");
            var md = MD5.Create();
            //var hmacmd5 = new HMACMD5(new UTF8Encoding().GetBytes("polievka"));
            var hash = md.ComputeHash(new UTF8Encoding().GetBytes(textBox1.Text));
            string l = BitConverter.ToString(hash).Replace("-", "").ToLower();
            //MessageBox.Show(l + "  |  " + pass);
            if (l == pass)
            {
                lh_updater.Properties.Settings.Default.pass = l;
                lh_updater.Properties.Settings.Default.Save();
                panel1.Hide();
                Update("_dev");

            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Application.ExitThread();
        }
    }
}
