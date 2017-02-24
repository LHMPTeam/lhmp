#pragma once

constexpr const char* DEFAULT_SERVER_NAME = "Default Lost Heaven Multiplayer Server";
constexpr const char* DEFAULT_SERVER_PASS = "";
constexpr int DEFAULT_SERVER_PORT = 27015;
constexpr int DEFAULT_MAX_PLAYERS = 32;

struct ServerProperties
{
	std::string mServerName;
	std::string mPassword;
	int mServerPort;
	int mMaxPlayers;
};

class Network;

class Core
{
public:
	Core();
	~Core();
	void Init();
	void Tick();
	void Log(const char* message, ...);
	Network* GetNetwork()
	{
		return mNetwork;
	}

	ServerProperties GetServerProperties() const
	{
		return mServerProperties;
	}

	bool IsRunning()
	{
		return mRunning;
	}
	static Core* GetCore();
private:
	Network* mNetwork;
	TickManager* mTickManager;
	ServerProperties mServerProperties;
	bool mRunning;
	static Core* CoreInstance;
};

