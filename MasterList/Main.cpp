// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
int main()
{
	printf("/***********************/\n");
	printf("/*  LHMP Masterserver  */\n");
	printf("/*  coded by Romop5    */\n");
	printf("/* %s %s 	/*\n",__DATE__,__TIME__);
	printf("/***********************/\n");
	printf("\n");
	bool result = CCore::getInstance().StartMaster();
	// it's has successfully started
	if (result)
	{
		printf("[OK] Master server has successfully started ...\n");
		for (;;)
		{
			CCore::getInstance().Pulse();
			Sleep(30);
		}
	}
	else {
		printf("[Err] Master server has failed to start ...\n");
	}
	return 0;
}
