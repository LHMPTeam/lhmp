/**
Lost Heaven Multiplayer

Purpose: handles all server-to-client filetransfers

@author Romop5
@version 1.0 1/9/14
*/

#ifndef FILETRANSFER_C
#define FILETRANSFER_C
#include <vector>
#include <fstream>
#include "RakPeerInterface.h"

// CFile handles data source (file)
class CFile
{
private:
	unsigned int ID;
	char name[255];
	FILE* handle;
	char checksum[33];
	int size;
public:
	CFile(unsigned int ID,FILE* handle, char name[]);
	~CFile();
	char*			GetName();
	char*			GetCheckSum();
	int				GetSize();
	unsigned int	GetID();
	FILE*			GetFileHandle();
};

class CTransmission
{
private:
	RakNet::SystemAddress	receiver;
	std::vector <CFile*>			fileList;

	int transferingID;
	int	transferingBytes;

	void	SendData();
	void	FinishIt();

public:
	CTransmission(RakNet::SystemAddress	receiver, std::vector <CFile*> list);
	void	HandlePacket(RakNet::BitStream* message);
	RakNet::SystemAddress GetReceiver();
};

class CFileTransfer
{
private:
	unsigned int					ID;
	std::vector <CFile*>			fileList;
	std::vector <CTransmission*>	clientList;

public:
	// Init
	CFileTransfer();
	// AddFile
	void	AddFile(char* sourcename,FILE* file);
	void	HandlePacket(RakNet::BitStream* message,RakNet::SystemAddress sa);

	void	SendFiles(RakNet::SystemAddress receiver);

	void	DeleteTransfer(CTransmission* tf, bool deleteIt = true);

	void	Reset();

};
#endif