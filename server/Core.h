#pragma once
#include "Network/Network.h"

class Core
{
public:
	Core();
	~Core();
	void Init();
	void Tick();
	Network* GetNetwork()
	{
		return mNetwork;
	}
private:
	Network* mNetwork;
};

