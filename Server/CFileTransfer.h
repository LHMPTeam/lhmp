// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef FILETRANSFER_C
#define FILETRANSFER_C
#include <vector>
#include <fstream>
#include "RakPeerInterface.h"

// CFile holds data(file) used during trasmission
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

/*
** Intern class: covers per-user trasmission of files
*/
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

/*
** Control class
*/
class CFileTransfer
{
private:
	unsigned int					ID;
	std::vector <CFile*>			fileList;
	std::vector <CTransmission*>	clientList;

public:
	// Init
	CFileTransfer();
	/*
	** Adds new file with @sourcename into list of files, streamed by server to clients
	** File has to be opened and file handled must be provided in @file
	*/
	void	AddFile(char* sourcename,FILE* file);
	/*
	** Send all streamed files to @receiver (usually on connection)
	*/
	void	SendFiles(RakNet::SystemAddress receiver);
	/*
	** Clear the list of streamed files (used during reloading)
	*/
	void	Reset();

	/* Intern system functions*/
	void	HandlePacket(RakNet::BitStream* message,RakNet::SystemAddress sa);
	void	DeleteTransfer(CTransmission* tf, bool deleteIt = true);


};
#endif