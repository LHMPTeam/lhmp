#include "CCore.h"

#include "CFileTransfer.h"
extern CCore *g_CCore;

void CFileTransfer::pStartTransfer()
{
	BitStream bsOut;
	bsOut.Write((MessageID)ID_FILETRANSFER_INIT);
	bsOut.Write(ID);
	bsOut.Write(transferType);
	bsOut.Write(filesCount);
	for (int i = 0; i < filesCount; i++)
	{
		bsOut.Write(fileList[i].name);
		bsOut.Write((int)0);
	}
	printf("StartTF \n");
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, receiver, false);
}

void CFileTransfer::pSendFile()
{
	BitStream bsOut;
	bsOut.Write((MessageID)ID_FILETRANSFER_SENDFILE);
	bsOut.Write(ID);
	bsOut.Write(fileProgress);

	/*int size = fileList[fileProgress].size - fileBytesSent;
	if (size < 200)
	{
		FILE* file = fopen(fileList[fileProgress].sourcename, "rb");
		if (file != NULL)
		{
			fread()
		}
	}*/
	//bsOut.Write("tralaladsadas");
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, receiver, false);
	printf("SendTF %d %s\n",fileProgress,fileList[fileProgress].name);
	fileBytesSent += this->fileList[fileProgress].size;
}

void CFileTransfer::pEndOfTransmission()
{
	BitStream bsOut;
	bsOut.Write((MessageID)ID_FILETRANSFER_TRANSFERDONE);
	bsOut.Write(ID);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, receiver, false);
	printf("EndTF \n");
}



// -------------------------------------------------------------------
//		PUBLIC FUNCTIONS

// Init
CFileTransfer::CFileTransfer(int ID, RakNet::SystemAddress receiver, unsigned short type)
{
	this->ID = ID;
	this->receiver = receiver;
	this->transferType = type;
	this->filesCount = 0;
	this->fileProgress = 0;
}
// AddFile
void	CFileTransfer::AddFile(char* sourcename, char* destination)
{
	std::ifstream currentFile(sourcename, std::ifstream::ate | std::ifstream::in);
	if (currentFile.is_open())
	{
		int bytesNum = currentFile.tellg();

		transferFile file;
		sprintf(file.name, "%s", sourcename);
		sprintf(file.sourcename, "%s", destination);
		fileList.push_back(file);
		file.size = bytesNum;
		filesCount++;
		currentFile.close();

		printf("Add file %s with size: %d \n", file.name, file.size);
	}
}

void	CFileTransfer::StartTransfer()
{
	this->pStartTransfer();
}

void	CFileTransfer::SendFile()
{
	if (fileProgress < filesCount)
	{
		if (fileBytesSent < fileList[fileProgress].size)
		{
			this->pSendFile();
		}
		else {
			fileProgress++;
			fileBytesSent = 0;
			this->pSendFile();
		}
	}
	else
	{
		this->pEndOfTransmission();
		// TODO - call callback
	}
}

int CFileTransfer::GetID()
{
	return this->ID;
}