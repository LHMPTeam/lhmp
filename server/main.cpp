#include "Core.h"
#include <thread>
#include <chrono>

int main(int argc, char * argv[])
{
	Core* core = new Core();
	core->Init();

	while (core->IsRunning())
	{
		using namespace std::chrono_literals;

		core->Tick();
		std::this_thread::sleep_for(10ms);
	}
}
