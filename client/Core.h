#pragma once

class Core
{
public:
	Core();
	~Core();
	void Init();
	void Tick();
	Game* GetGame() { return mGame; };
	bool IsRunning() { return mIsRunning; };
	Network* GetNetwork() { return mNetwork; };
	static Core* GetCore();
private:
	Network* mNetwork;
	Game* mGame;
	bool mIsRunning;
	static Core* CoreInstance;
};

