/**
	Lost Heaven Multiplayer - shared

	Purpose: global version information 
*/
#define _DEBUG

#define LHMP_VERSION_MAJOR	0
#define LHMP_VERSION_MINOR	1
#define LHMP_VERSION_TYPE	VERSION_TEST

// when LHMP is compiled as test version
#define LHMP_VERSION_TEST_HASH "abcac21c057f0eabceaa8d8fafa72955"

enum versionTypes
{
	VERSION_TEST,
	VERSION_PUBLIC
};