#include "../sdks/UDPWrapper/UDPWrapper.h"
#include "CCore.h"


int main()
{
	bool result = CCore::getInstance().StartMaster();
	// it's has successfully started
	if (result)
	{
		printf("[OK] Master server has successfully started ...\n");
		for (;;)
		{
			CCore::getInstance().Pulse();
		}
	}
	else {
		printf("[Err] Master server has failed to start ...\n");
	}
	return 0;
}