/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose bansystem - loading banlist, adding bans
@author Romop5
******************************************************************************/
class CBanSystem
{
public:
	CBanSystem();
	// Loads banlist from ban file (during initialization)
	void LoadBanlist();
	// Add ban
	// @IPaddres in IPv4 format XXX.XXX.XXX.XXX
	// @reason why ban is given to player
	// @duration in seconds
	void AddBan(char* IPaddres,char* reason,unsigned int duration);
private:
	void ParseLineFromFile(char line[],int lineID);
	void AddBanToFile(char* IPaddres, unsigned int duration, char* reason = "", char* note = "");
	unsigned int p_banCount;
};