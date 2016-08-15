// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "squirrelheads.h"
#include <cstring>
class CScript
{
private:
	SQVM * m_pVM;
	char m_szScriptName[256];
	char m_szScriptAuthor[256];
	char m_szScriptVersion[256];
public:
	CScript(const char * szScriptName);
	~CScript();

	SQVM * GetVM() { return m_pVM; };

	char * GetScriptName() { return (char*)&m_szScriptName; };
	char * GetScriptAuthor() { return (char*)&m_szScriptAuthor; };
	char * GetScriptVersion() { return (char*)&m_szScriptVersion; };
	void SetScriptAuthor(const char * szAuthor) { strncpy(m_szScriptAuthor, szAuthor, sizeof(m_szScriptAuthor)); };
	void SetScriptVersion(const char * szVersion) { strncpy(m_szScriptVersion, szVersion, sizeof(m_szScriptVersion)); };
};
