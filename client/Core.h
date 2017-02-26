#pragma once

class Core
{
public:
	Core();
	~Core();
	void Init();
	void Tick();
	bool IsRunning() { return mIsRunning; };

	Game* GetGame() { return mGame; };
	Network* GetNetwork() { return mNetwork; };
	Graphics* GetGraphics() { return mGraphics; }

	static Core* GetCore();
private:
	static Core* CoreInstance;
	Network* mNetwork;
	Game* mGame;
	Graphics* mGraphics;
	bool mIsRunning;
	
};

