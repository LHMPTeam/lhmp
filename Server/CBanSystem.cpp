#include "CCore.h"

CBanSystem::CBanSystem()
{

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
			this->ParseLineFromFile(pointer);
			pointer = strtok(NULL, "\r\n\0");
		}
		// close file
		fclose(file);
	}
}

void CBanSystem::AddBan(char* IPaddres, char* reason, unsigned int duration)
{
	
}

// ---------------------- PRIVATE ------------------------//

void CBanSystem::ParseLineFromFile(char line[255])
{
	char newInput[1024];

	int len = strlen(line);
	int state = 0;
	for (int i = 0; i < len; i++)
	{
		if (line[i] == ';')
		{
			break;
		}
		else if (line[i] == ' ' || line[i] == '	')
		{
			if (newInput[state - 1] != ' ')
			{
				newInput[state] = ' ';
				state++;
			}
		}
		else {
			newInput[state] = line[i];
			state++;
		}
	}

	// newInput now contains optimal input (no multiple whitespaces & TABs)

	if (strlen(newInput) == 0)
		return;

	for (int i = 0; i < strlen(newInput); i++)
	{
		if (newInput[i] == ' ')
		{

		}
	}
	
}

void CBanSystem::AddBanToFile(char* IPaddres, unsigned int duration, char* reason)
{

}