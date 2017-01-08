#pragma once
#include <Utils.h>
#include <ini.hpp>


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

	static Core* GetCore();
private:
	Network* mNetwork;
	ServerProperties mServerProperties;

	static Core* CoreInstance;
};

