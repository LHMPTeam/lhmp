/**
	Lost Heaven Multiplayer - shared

	Purpose: global version information 
*/
#define _DEBUG

#define LHMP_VERSION_MAJOR	0
#define LHMP_VERSION_MINOR	1
#define LHMP_VERSION_TYPE	VERSION_TEST

// when LHMP is compiled as test version
#define LHMP_VERSION_TEST_HASH "f80e021c057f0eabeeee8d8faf372903"

enum versionTypes
{
	VERSION_TEST,
	VERSION_PUBLIC
};