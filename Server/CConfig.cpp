#include "CConfig.h"
#include <cstring>
#include <cstdlib>

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
	ConfigItem* workwith;
	ConfigItem* next = start;
	while (next != 0)
	{
		workwith = next;
		next = next->next;
		delete workwith;
	}
}


void	CConfig::LoadConfig()
{
	char buff[500];
	std::fstream file;
	file.open("config.txt", std::ios::in);
	if (file.is_open())
	{
		do {
			file.getline(buff, 500);
			AddItem(buff);
		} while (file.eof() != true);
	}
	file.close();
}


void	CConfig::AddItem(char* buff)
{
	int pos = 0;
	char endbuff[500];
	//std::cout << "Buff: '" << buff << "'"<< std::endl;
	for (int i = 0; i < 500; i++)
	{
		if (buff[i] == ';')
		{
			endbuff[pos] = '\0';
			break;
		}
		else if ((buff[i] != '	') && (buff[i] != ' '))
			endbuff[pos++] = buff[i];
		else {
			if (pos)
			{
				if (endbuff[pos - 1] != ' ')
					endbuff[pos++] = ' ';
			}
		}
	}
	if (pos == 0 || (pos == 1 && endbuff[0] == ' ') || strlen(endbuff) == 0)
		return;
	//std::cout << "BuffEnd: '" << endbuff << "' " << strlen(endbuff) << std::endl;

	for (int i = 0; i < pos; i++)
	{
		if (endbuff[i] == ' ')
		{
			pos = i;
			break;
		}
	}
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

int		CConfig::GetInt(char* name, int def)
{
	if (itemCount == 0)
		return def;

	ConfigItem* ci = start;
	while (ci != 0)
	{
		if (strcmp(ci->name, name) == 0)
		{
			return atoi(ci->content);
		} else {
			ci = ci->next;
		}
	}
	return def;
}
bool	CConfig::GetBool(char* name, bool def)
{
	if (itemCount == 0)
		return def;

	ConfigItem* ci = start;
	while (ci != 0)
	{
		if (strcmp(ci->name, name) == 0)
		{
			return (atoi(ci->content) == 1);
		}
		else {
			ci = ci->next;
		}
	}
	return def;
}
char*	CConfig::GetCString(char* name, char* def)
{
	if (itemCount == 0)
		return def;

	ConfigItem* ci = start;
	while (ci != 0)
	{
		if (strcmp(ci->name, name) == 0)
		{
			char* result = new char[strlen(ci->content) + 1];
			strcpy(result, ci->content);
			return result;
		} else {
			ci = ci->next;
		}
	}
	return def;
}
