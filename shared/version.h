// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#define _DEBUG

#define LHMP_VERSION_MAJOR	0
#define LHMP_VERSION_MINOR	1
#define LHMP_VERSION_TYPE	VERSION_PUBLIC

// when LHMP is compiled as test version
#define LHMP_VERSION_TEST_HASH "EXP_PREVIEW_0002_08242015"

// protocol version (byte) 0-255
#define LHMP_QUERY_PROTOCOL	0

#define LHMP_TITLE "Lost Heaven Multiplayer"
enum versionTypes
{
	VERSION_TEST,
	VERSION_PUBLIC
};

#ifdef _WIN32
#define LHMP_PLATFORM "Windows"
#else
#define LHMP_PLATFORM "Linux"
#endif