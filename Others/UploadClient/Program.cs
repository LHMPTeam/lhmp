using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace UploadClient
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                if (args[0] == "dev") { UpdateDev(); }
                else UpdatePublic();
            }
            catch
            {
                UpdatePublic();
            }
        }


        static void UpdateDev()
        {
            if (!File.Exists("client.dll")) { return; }
            try
            {
                WebRequest requestFTPx = WebRequest.Create("ftp://" + "lh-mp.eu" + "/" + "web/_updater/files_dev/lhmp/client.dll");
                requestFTPx.Credentials = new NetworkCredential("lhup", "himalajeddo");
                requestFTPx.Method = WebRequestMethods.Ftp.DeleteFile;
                FtpWebResponse response = (FtpWebResponse)requestFTPx.GetResponse();
                Console.WriteLine("Removing client.dll...");
            }
            catch { }
            try
            {
                Console.WriteLine("Uploading client.dll...");
                FileInfo fileInfo = new FileInfo("client.dll");
                string fileName = fileInfo.Name.ToString();
                WebRequest requestFTP = WebRequest.Create("ftp://" + "lh-mp.eu" + "/" + "web/_updater/files_dev/lhmp/client.dll");
                requestFTP.Credentials = new NetworkCredential("lhup", "himalajeddo");
                requestFTP.Method = WebRequestMethods.Ftp.UploadFile;
                FileStream fStream = fileInfo.OpenRead();
                int bufferLength = 2048;
                byte[] buffer = new byte[bufferLength];

                Stream uploadStream = requestFTP.GetRequestStream();
                int contentLength = fStream.Read(buffer, 0, bufferLength);

                while (contentLength != 0)
                {
                    uploadStream.Write(buffer, 0, contentLength);
                    contentLength = fStream.Read(buffer, 0, bufferLength);
                }
                uploadStream.Close();
                fStream.Close();
                requestFTP = null;
                //MessageBox.Show("File Uploading Is SuccessFull...");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString()); Console.ReadKey();
            }
        }

        static void UpdatePublic()
        {
            if (!File.Exists("client.dll")) { return; }
            try
            {
                WebRequest requestFTPx = WebRequest.Create("ftp://" + "lh-mp.eu" + "/" + "web/_updater/files/lhmp/client.dll");
                requestFTPx.Credentials = new NetworkCredential("lhup", "himalajeddo");
                requestFTPx.Method = WebRequestMethods.Ftp.DeleteFile;
                FtpWebResponse response = (FtpWebResponse)requestFTPx.GetResponse();
                Console.WriteLine("Removing client.dll...");
            }
            catch { }
            try
            {
                Console.WriteLine("Uploading client.dll...");
                FileInfo fileInfo = new FileInfo("client.dll");
                string fileName = fileInfo.Name.ToString();
                WebRequest requestFTP = WebRequest.Create("ftp://" + "lh-mp.eu" + "/" + "web/_updater/files/lhmp/client.dll");
                requestFTP.Credentials = new NetworkCredential("lhup", "himalajeddo");
                requestFTP.Method = WebRequestMethods.Ftp.UploadFile;
                FileStream fStream = fileInfo.OpenRead();
                int bufferLength = 2048;
                byte[] buffer = new byte[bufferLength];

                Stream uploadStream = requestFTP.GetRequestStream();
                int contentLength = fStream.Read(buffer, 0, bufferLength);

                while (contentLength != 0)
                {
                    uploadStream.Write(buffer, 0, contentLength);
                    contentLength = fStream.Read(buffer, 0, bufferLength);
                }
                uploadStream.Close();
                fStream.Close();
                requestFTP = null;
                //MessageBox.Show("File Uploading Is SuccessFull...");
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString()); Console.ReadKey();
            }
        }
    }
}