#include "CFileSystem.h"
#include "CCore.h"

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <ios>

extern CCore* g_CCore;

void CFileSystem::Init(const char* text)
{
	sprintf(this->path, "%s", text);
}

CFileSystem::CFileSystem()
{
	
}

CFileSystem::~CFileSystem()
{

}

void CFileSystem::iniGetParam(const char* file, const char* param, char* value)
{
	char filepath[256] = "";
	sprintf(filepath, "%s%s", this->path, file);
	std::ifstream fp(filepath);
	std::string line;
	while (std::getline(fp, line))
	{
		std::istringstream iss(line);
		std::string null, tmp;

		if ((iss >> null >> tmp))
		{
			char cnull[256] = "";
			sprintf(cnull, "%s", null.c_str());
			if (!strcmp(cnull, param))
			{
				char out[256] = "";
				int offset = strlen(param) + 1;
				for (int i = 0; i < (strlen(line.c_str()) - strlen(param) - 1); i++)
				{
					out[i] = line.c_str()[i+offset];
				}
				sprintf(value, "%s",out);
				return;
			}
		}
	}
	sprintf(value, "");
	return;
}
void CFileSystem::iniSetParam(const char* file, const char* param, const char* value)
{
	bool isAppend = true;
	char filepath[256] = "";
	sprintf(filepath, "%s%s", this->path, file);
	std::ifstream fp(filepath);
	std::string line,buffer;
	while (std::getline(fp, line))
	{
		buffer += line + '\n'; // Populates my buffer with data from file.
	}
	fp.close();

	std::fstream fo;
	fo.open(filepath, std::ios::out);
	std::istringstream b(buffer);

	while (std::getline(b, line))
	{
		std::istringstream iss(line);
		std::string null, tmp;
		if ((iss >> null >> tmp))
		{
			char cnull[256] = "";
			sprintf(cnull, "%s", null.c_str());
			if (!strcmp(cnull, param))
			{
				char out[256] = "";
				sprintf(out, "%s %s", param, value);
				fo << std::string(out) << std::endl;
				fo.flush();
				isAppend = false;
				continue;
			}
		}
		fo << line << std::endl;
		fo.flush();
	}

	if (isAppend)
	{
		char out[256] = "";
		sprintf(out, "%s %s", param, value);
		fo << out << std::endl;
		fo.flush();
	}

	return;
}
void CFileSystem::iniRemoveFile(const char* file)
{
	char path[256];
	sprintf(path, "%s%s", this->path, file);
	int ret_code = std::remove(path);
	if (ret_code != 0) 
	{
		g_CCore->GetLog()->AddNormalLog("[FS] Can't remove file: %s (%d).", file, ret_code);
	}
}

void CFileSystem::iniCreateFile(const char* file)
{
	std::ofstream of(file);
	of << '\0';
}