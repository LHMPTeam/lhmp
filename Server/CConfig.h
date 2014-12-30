/**
Lost Heaven Multiplayer

Purpose: config.txt 

@author Romop5
@version 1.0 1/9/14
*/

#include <iostream>
#include <fstream>
struct ConfigItem
{
	char name[255];
	char content[255];
	ConfigItem* next;
};
class CConfig
{
private:
	ConfigItem* start;
	ConfigItem* end;
	int			itemCount;
	// Parse config file
	void		LoadConfig();
	// Parse line from config
	void		AddItem(char*);
	// Get config item with specified name
	ConfigItem*	GetItemWithName(char*);
public:
	CConfig();
	~CConfig();
	// Get integer value
	// @name is config item's name
	// @def default value if item doesn't exists
	int			GetInt(char* name, int def);
	// Get bool value
	// @name is config item's name
	// @def default value if item doesn't exists
	bool		GetBool(char* name, bool def);
	// Get cstring
	// @name is config item's name
	// @def default value if item doesn't exists
	char*		GetCString(char* name, char* def);
};