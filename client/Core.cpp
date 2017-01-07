#include "Core.h"

Core::Core()
{
}


Core::~Core()
{
}

void Core::Init()
{
	mNetwork = new Network();
	mNetwork->Init();
}

void Core::Tick()
{
	mNetwork->Tick();
}
