#include <stdinc.h>

Core::Core()
{
}


Core::~Core()
{
}

void Core::Init()
{
	/*mNetwork = new Network();
	mNetwork->Init();*/
	mGame = new Game();
	mGame->Init();
}

void Core::Tick()
{
	mNetwork->Tick();
}
