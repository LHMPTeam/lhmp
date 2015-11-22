#include "CCore.h"
#include "CConfig.h"
#include <cstring>
#include <cstdlib>

extern CCore *g_CCore;

CConfig::CConfig()
{
	start	= 0;
	end		= 0;
	itemCount = 0;

	// lets load conf
	LoadConfig();
}

CConfig::~CConfig()
{
	// just delete dynamically allocated memory
	ConfigItem* next = start;
	while (next != 0)
	{
		ConfigItem* workwith = next;
		next = next->next;
		delete workwith;
	}
}


void	CConfig::LoadConfig()
{
	// open our config
	FILE* file = fopen("config.txt", "rb");
	// is file opened
	if (file != NULL)
	{
		//printf("[Success] Cofig file opened\n");
		// calculate file size
		fseek(file, 0, SEEK_END);   // non-portable
		int size = ftell(file);
		fseek(file, 0, SEEK_SET);
		// now allocate enough memory to load whole file
		char *fileContent = new char[size + 1];
		fread(fileContent, sizeof(char), size, file);
		fileContent[size] = 0x0;
		// now we are going to split file to lines
		char* pointer;
		// \r\n\0 should work as universal file-ending 
		pointer = strtok(fileContent, "\r\n\0");
		// while we have lines
		while (pointer != NULL)
		{
			// parse line
			AddItem(pointer);
			pointer = strtok(NULL, "\r\n\0");
		}
		// close file & free allocated memory
		fclose(file);
		delete[] fileContent;
	}
	else {
		g_CCore->GetLog()->AddNormalLog("[Error] Cofig file failed to load\n");
	}
}

// Parse lines from config file
void	CConfig::AddItem(char* buff)
{
	int pos = 0;
	char endbuff[500];
	for (int i = 0; i < 500; i++)
	{
		// if line contains ';', line ends at that char
		if (buff[i] == ';')
		{
			endbuff[pos] = '\0';
			break;
		}
		// if character is white-space or tab
		else if ((buff[i] != '\t') && (buff[i] != ' '))
			endbuff[pos++] = buff[i];
		else {
			if (pos)
			{
				if (endbuff[pos - 1] != ' ')
					endbuff[pos++] = ' ';
			}
		}
	}
	// if line is empty
	if (pos == 0 || (pos == 1 && endbuff[0] == ' ') || strlen(endbuff) == 0)
		return;
	// split line into 2 informations (using blank space)
	for (int i = 0; i < pos; i++)
	{
		if (endbuff[i] == ' ')
		{
			pos = i;
			break;
		}
	}
	// create new config item with name & value
	ConfigItem* itm = new ConfigItem();
	strncpy(itm->name, endbuff, pos++);
	strncpy(itm->content, endbuff+pos,255);
	if (end != 0)
	{
		end->next = itm;
		end = itm;
	} else {
		start = itm;
		end = start;
	}
	itemCount++;
}


ConfigItem*	CConfig::GetItemWithName(char* name)
{
	if (itemCount == 0)
		return NULL;
	ConfigItem* ci = start;
	while (ci != 0)
	{
		if (strcmp(ci->name, name) == 0)
		{
			return ci;
		}
		else {
			ci = ci->next;
		}
	}
	return NULL;
}

int		CConfig::GetInt(char* name, int def)
{
	ConfigItem* item = this->GetItemWithName(name);
	if (item)
	{
		return atoi(item->content);
	}
	return def;
}
bool	CConfig::GetBool(char* name, bool def)
{
	ConfigItem* item = this->GetItemWithName(name);
	if (item)
	{
		return (atoi(item->content) == 1);
	}
	return def;
}
char*	CConfig::GetCString(char* name, char* def)
{
	ConfigItem* item = this->GetItemWithName(name);
	if (item)
	{
		char* result = new char[strlen(item->content) + 1];
		strcpy(result, item->content);
		return result;
	}
	return def;
}
