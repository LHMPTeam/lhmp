#include "CCore.h"
#include "CFileTransfer.h"
#include "../sdks/md5/md5.h"
#include "../shared/structures.h"

#include "BitStream.h"

extern CCore* g_CCore;

CFile::CFile(unsigned int _ID,FILE* _handle, char _name[])
{
	this->ID = _ID;
	this->handle = _handle;
	strcpy(this->name, _name);

	// calculate MD5
	MD5File(_handle,this->GetCheckSum());

	// get size
	fseek(_handle, 0, SEEK_END);   // non-portable
	this->size = ftell(_handle);

	rewind(_handle);
}
char* CFile::GetName()
{
	return this->name;
}

char*	CFile::GetCheckSum()
{
	return this->checksum;
}

int		CFile::GetSize()
{
	return this->size;
} 

unsigned int	CFile::GetID()
{
	return this->ID;
}

FILE*	CFile::GetFileHandle()
{
	return this->handle;
}
//******************************************************************************


CTransmission::CTransmission(RakNet::SystemAddress	receiver, std::vector <CFile*> list)
{
	this->fileList.clear();
	this->receiver = receiver;
	this->transferingBytes = 0;
	this->transferingID = 0;

	for (unsigned int i = 0; i < list.size(); i++)
		this->fileList.push_back(list[i]);
	//this->fileList(list.begin(),list.end);

	// send list
	RakNet::BitStream out;
	out.Write((RakNet::MessageID)ID_FILETRANSFER);
	out.Write((RakNet::MessageID)FILETRANSFER_INIT);
	out.Write((int)this->fileList.size());						// file count
	//printf("Files count %d \n", (int)this->fileList.size());
	for (unsigned int i = 0; i < this->fileList.size(); i++)
	{
		out.Write(this->fileList[i]->GetID());
		out.Write(this->fileList[i]->GetCheckSum());
		out.Write(this->fileList[i]->GetName());
		out.Write(this->fileList[i]->GetSize());

		//printf("CTransmission \n");
	}
	g_CCore->GetNetworkManager()->GetPeer()->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, this->receiver, false);
}

void	CTransmission::SendData()
{
	//printf("[Out] transferingID %d %d\n", this->transferingID,this->fileList.size());
	if ((unsigned int) this->transferingID < this->fileList.size())
	{

		FILE* file = this->fileList[this->transferingID]->GetFileHandle();

		//unsigned char buff[65537];		// 512 kb
		unsigned char data[65537];
		fseek(file, this->transferingBytes, SEEK_SET);	// set position
		int result = fread(data, 1, 65537, file);
		//printf("Read bytes %d \n", result);

		this->transferingBytes += result;
		if (ferror(file))
		{
			// oh, fuck
		}
		else {
			// send data
			RakNet::BitStream out;
			out.Write((RakNet::MessageID)ID_FILETRANSFER);
			out.Write((RakNet::MessageID)FILETRANSFER_SEND);
			int ID = this->fileList[this->transferingID]->GetID();
			//printf("Send file ID: %d \n", ID);

			out.Write(ID);
			out.Write(result);
			for (int i = 0; i < (result); i++)
				out.Write(data[i]);
			g_CCore->GetNetworkManager()->GetPeer()->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, this->receiver, false);


			if (result < 65537)
			{
				this->transferingID++;
				this->transferingBytes = 0;
			}
		}
	}
	else {
		this->FinishIt();
	}
}

void	CTransmission::FinishIt()
{
	//printf("[Out] SendFinish\n");
	RakNet::BitStream out;
	out.Write((RakNet::MessageID)ID_FILETRANSFER);
	out.Write((RakNet::MessageID)FILETRANSFER_FINISH);
	out.Write(this->transferingID);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, this->receiver, false);
}

void	CTransmission::HandlePacket(RakNet::BitStream* pMsg)
{
	unsigned char msgType;
	pMsg->Read(msgType);
	switch (msgType)
	{
		case FILETRANSFER_INIT:
		{
			//printf("[In] CFileTransfer Init \n");
			// assume that the list is ordered (from the smallest to the biggest ID)
			int lastOne = -1;
			int filesCount;
			pMsg->Read(filesCount);
			std::vector<CFile*> newlist;
			for (int i = 0; i < filesCount; i++)
			{
				int ID;
				pMsg->Read(ID);
				for (unsigned int i = 0; i < this->fileList.size(); i++)
				{
					if (this->fileList[i]->GetID() == ID)
					{
						newlist.push_back(this->fileList[i]);
						break;
					}
				}
			}

			// just copy
			this->fileList.clear(); 
			for ( unsigned int i = 0; i < newlist.size(); i++)
				this->fileList.push_back(newlist[i]);

			this->SendData();
		}
			break;
		case FILETRANSFER_SEND:
		{
			//printf("[In] CFileTransfer SendFiles \n");
			this->SendData();
		}
			break;
		case FILETRANSFER_FINISH:
		{
			// call our callback
			//printf("[In] CFileTransfer Finish \n");
			g_CCore->GetNetworkManager()->OnPlayerFileTransferFinished(this->receiver);
			g_CCore->GetFileTransfer()->DeleteTransfer(this, false);
			delete this;
			return;
		}
			break;
		default:
			break;
	}
}

RakNet::SystemAddress CTransmission::GetReceiver()
{
	return this->receiver;
}

//******************************************************************************
CFileTransfer::CFileTransfer()
{
	this->ID = 0;
}

void	CFileTransfer::AddFile(char* _sourcename, FILE* _file)
{
	CFile* file = new CFile(this->ID,_file, _sourcename);
	this->fileList.push_back(file);
	this->ID++;
}
void	CFileTransfer::HandlePacket(RakNet::BitStream* message, RakNet::SystemAddress sa)
{
	for (unsigned int i = 0; i < clientList.size(); i++)
	{
		if (this->clientList[i]->GetReceiver() == sa)
			this->clientList[i]->HandlePacket(message);
	}
}

void	CFileTransfer::SendFiles(RakNet::SystemAddress receiver)
{
	CTransmission* transfer = new CTransmission(receiver, this->fileList);
	this->clientList.push_back(transfer);
}

void	CFileTransfer::DeleteTransfer(CTransmission* tf, bool deleteIt)
{
	for (unsigned int i = 0; i < this->clientList.size(); i++)
	{
		if (this->clientList[i] == tf)
		{
			if (deleteIt)
				delete this->clientList[i];
			this->clientList.erase(this->clientList.begin() + i);
		}
	}
}