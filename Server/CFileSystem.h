#ifndef CFILESYSTEM_H
#define CFILESYSTEM_H

class CFileSystem
{
private:
	char path[256];
public:
	CFileSystem();
	~CFileSystem();
	void Init(const char*);
	void iniGetParam(const char*, const char*, char*);
	void iniSetParam(const char*, const char*, const char*);
	void iniRemoveFile(const char*);
	void iniCreateFile(const char*);
};

#endif