class CBanSystem
{
public:
	CBanSystem();
	// Loads banlist from file
	void LoadBanlist();
	// Add ban
	// @IPaddres in format XXX.XXX.XXX.XXX
	// @reason why player got ban
	// @duration in seconds
	void AddBan(char* IPaddres,char* reason,unsigned int duration);
private:
	void ParseLineFromFile(char line[],int lineID);
	void AddBanToFile(char* IPaddres, unsigned int duration, char* reason = "", char* note = "");
};