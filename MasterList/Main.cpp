//#include "../sdks/UDPWrapper/UDPWrapper.h"
#include "CCore.h"


int main()
{
	printf("/***********************/\n");
	printf("/*  LHMP Masterserver  */\n");
	printf("/*  coded by Romop5    */\n");
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
			Sleep(10);
		}
	}
	else {
		printf("[Err] Master server has failed to start ...\n");
	}
	return 0;
}