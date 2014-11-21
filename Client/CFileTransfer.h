#include "CCore.h"
#include "FileListTransferCBInterface.h"

extern CCore* g_CCore;

class CFileTransfer : public  RakNet::FileListTransferCBInterface
{
	bool OnFile(OnFileStruct *onFileStruct)
	{
		g_CCore->GetChat()->AddMessage("OnFile");
		return true;
	}
	void OnFileProgress(FileProgressStruct *fps)
	{

		g_CCore->GetChat()->AddMessage("OnFileProgress");
	}
	bool 	Update(void)
	{

		//g_CCore->GetChat()->AddMessage("FT Update");
		return true;
	}
	bool 	OnDownloadComplete(DownloadCompleteStruct *dcs)
	{
		char buff[255];
		g_CCore->GetChat()->AddMessage("OnDownloadComplete");
		sprintf(buff, "Files count: %u Whole length: %u", dcs->numberOfFilesInThisSet, dcs->byteLengthOfThisSet);
		g_CCore->GetChat()->AddMessage(buff);
		return true;
	}
	void 	OnDereference(void)
	{

	}

};