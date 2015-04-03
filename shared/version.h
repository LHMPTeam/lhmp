/**
	Lost Heaven Multiplayer - shared

	Purpose: global version information 
*/
// that means we are running in debug mode
#define _DEBUG

#define LHMP_VERSION_MAJOR	0
#define LHMP_VERSION_MINOR	1
#define LHMP_VERSION_TYPE	VERSION_TEST

// when LHMP is compiled as test version
#define LHMP_VERSION_TEST_HASH "TEST11ca2a00f92eb23917628c3cd453"

// protocol version (byte) 0-255
#define LHMP_QUERY_PROTOCOL	0
enum versionTypes
{
	VERSION_TEST,
	VERSION_PUBLIC
};