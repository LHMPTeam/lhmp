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
	void		LoadConfig();
	void		AddItem(char*);
public:
	CConfig();
	~CConfig();
	int			GetInt(char* name, int def);
	bool		GetBool(char* name, bool def);
	char*		GetCString(char* name, char* def);
};