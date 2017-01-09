#pragma once

class Core
{
public:
	Core();
	~Core();
	void Init();
	void Tick();
	Network* GetNetwork() { return mNetwork; };
	Game* GetGame() { return mGame; };
	bool IsRunning() { return mIsRunning; };
	static Core* GetCore();
private:
	Network* mNetwork;
	Game* mGame;
	bool mIsRunning;
	static Core* CoreInstance;
};

