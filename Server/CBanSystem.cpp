// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include <time.h>
extern CCore* g_CCore;
CBanSystem::CBanSystem()
{
	p_banCount = 0;
}

// Load bans from "banlist.txt"
void CBanSystem::LoadBanlist()
{
	// open our config
	FILE* file = fopen("banlist.txt", "rb");
	// is file opened
	if (file != NULL)
	{
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
		int lineNumber = 0;
		// while we have lines
		while (pointer != NULL)
		{
			// parse line
			this->ParseLineFromFile(pointer,++lineNumber);
			pointer = strtok(NULL, "\r\n\0");
		}
		// close file
		fclose(file);
		delete[] fileContent;
	}
	g_CCore->GetLog()->AddNormalLog("[Banlist] %d bans loaded.",this->p_banCount);
}

// IP address, reason, duration (seconds)
void CBanSystem::AddBan(char* IPaddres, char* reason, unsigned int duration)
{
	g_CCore->GetLog()->AddNormalLog("[BanList] IP[%s] banned for %d secs due to reason: '%s'",IPaddres,duration,reason);
	//
	unsigned int milisec = duration * 1000;
	// Add ban to network bad list
	g_CCore->GetNetworkManager()->GetPeer()->AddToBanList(IPaddres, milisec);
	// Write a new ban down into banlist.txt
	this->AddBanToFile(IPaddres, milisec, reason);
}

// ---------------------- PRIVATE ------------------------//

void CBanSystem::ParseLineFromFile(char* line,int lineID)
{
	time_t  timev;
	time(&timev);
	unsigned int banTime;
	//g_CCore->GetLog()->AddNormalLog("Line: %s", line);
	// four items with max length 200
	// items: (compulsory) IP, (compulsory) lasting, (optional) reason_for_ban,(optional) special_note
	// reason and special note might be empty
	char items[4][200];
	// fill all strings with ZEROs, useful for floating ending chars
	memset(items, 0x0, 4 * 200);

	// example line
	// "127.0.0.1"	"1420302218"	"dovod na ban"	""

	// find first ,", occurence
	int length = strlen(line);
	int pointer = 0,count = 0;
	while (count <= 3)
	{
		// whether we found the first or second "
		bool bracesToggle = false;
		// holds the first occurence of "
		int firstBrace = 0;
		for (; pointer < length; pointer++)
		{
			if (line[pointer] == ';')
			{
				if (bracesToggle == false)
					//g_CCore->GetLog()->AddNormalLog("[Error] Banlist, line %d - missing \" to seperate data. Found ';'", lineID);
				// anyway, break here
				break;
			} else if (line[pointer] == '"')
			{
				// if found the second brace, then copy and jump into next round
				if (bracesToggle)
				{
					memcpy(items[count], line + firstBrace, pointer - firstBrace);
					bracesToggle = !bracesToggle;
					pointer++;
					break;
				}
				else {
					// we found the first one, so toggle the switch
					firstBrace = pointer+1;
					bracesToggle = !bracesToggle;
				}
			}
		}
		// if we have found first brace, but second is missing, show an error
		if (bracesToggle == true)
			g_CCore->GetLog()->AddNormalLog("[Error] Banlist, line %d:%d - missing \" to seperate data.", lineID,firstBrace);
		count++;
		// break if a comment was found
		if (line[pointer] == ';')
			break;
	}

	// lets see
	//g_CCore->GetLog()->AddNormalLog("Read data: A'%s' | B'%s' | C'%s' | D'%s'", items[0], items[1], items[2], items[3]);

	if (strlen(items[0]) > 0 && strlen(items[1]) > 0)
	{
		sscanf(items[1], "%u", &banTime);
		if (banTime > timev)
		{
			banTime -= (unsigned int)timev;
			g_CCore->GetNetworkManager()->GetPeer()->AddToBanList(items[0], banTime);
			this->p_banCount++;
			//g_CCore->GetLog()->AddNormalLog(">>>Adding ban - %s.",items[0]);
		}
	}
}


void CBanSystem::AddBanToFile(char* IPaddres, unsigned int duration, char* reason,char* note)
{
	FILE* file = fopen("banlist.txt", "a+");
	if (file != NULL)
	{

		char line[500];
		char timestamp[20];
		time_t  timev;
		time(&timev);
		sprintf(timestamp, "%u", timev + duration);
		
		sprintf(line, "\"%s\" \"%s\" \"%s\" \"%s\"\n", IPaddres, timestamp, reason, note);
		fwrite(line,1,strlen(line),file);

		fclose(file);
	}
}