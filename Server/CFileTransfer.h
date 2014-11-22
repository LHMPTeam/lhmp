#ifndef FILETRANSFER_C
#define FILETRANSFER_C
#include <vector>
#include <fstream>

struct transferFile
{
	char sourcename[250];
	char name[250];
	int size;
};
class CFileTransfer
{
private:
	int						ID;
	RakNet::SystemAddress	receiver;
	unsigned short			transferType;
	int						filesCount;
	std::vector <transferFile>	fileList;

	int						fileProgress;
	int						fileBytesSent;

	void pStartTransfer();
	void pSendFile();
	void pEndOfTransmission();
public:
	// Init
	CFileTransfer(int ID, RakNet::SystemAddress receiver, unsigned short type);
	// AddFile
	void	AddFile(char* sourcename, char* destination);
	void	StartTransfer();
	void	SendFile();
	int		GetID();

};
#endif