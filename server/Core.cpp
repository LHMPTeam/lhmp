#include "Core.h"
#include "Network\Network.h"

Core::Core()
{
	CoreInstance = this;
}


Core::~Core()
{
}

void Core::Log(const char* format, ...)
{
	va_list ap;
	char message[1024] = { 0 };
	va_start(ap, format);
	vsprintf(message, format, ap);
	va_end(ap);
	printf("[SERVER] [%s] - %s\n", currentDateTime().c_str(), message);
}

void Core::Init()
{
	std::ifstream configFile("config.ini", std::ios::in);
	INI::Parser configParser(configFile);
	
	mServerProperties.mMaxPlayers = atoi(configParser.top()("Server")["MaxPlayers"].c_str());
	mServerProperties.mServerName = configParser.top()("Server")["ServerName"];
	mServerProperties.mPassword = configParser.top()("Server")["ServerPass"];
	mServerProperties.mServerPort = atoi(configParser.top()("Server")["ServerPort"].c_str());
	
	std::string test = R"aaa(                                                
				 _      _   _            ___   
				( )    ( ) ( )   /'\_/`\(  _`\    VERSION: 1.0.0 INTERNAL
				| |    | |_| | _ |     || |_) )
				| |  _ |  _  |(_)| (_) || ,__/'
				| |_( )| | | | _ | | | || |    
				(____/'(_) (_)(_)(_) (_)(_)  )aaa";

	printf("%s\n\n", test.c_str());

	if (mServerProperties.mMaxPlayers < 1 || mServerProperties.mMaxPlayers > 666)
	{
		Log("Unable to get 'MaxPlayers' from config file ! Using default value: '%d'", DEFAULT_MAX_PLAYERS);
		mServerProperties.mMaxPlayers = DEFAULT_MAX_PLAYERS;
	}

	if (!mServerProperties.mPassword.size())
	{
		Log("Unable to get 'ServerPass' from config file !");
		mServerProperties.mPassword = "";
	}

	if (!mServerProperties.mServerName.size())
	{
		Log("Unable to get 'ServerName' from config file ! Using default value: '%s'", DEFAULT_SERVER_NAME);
		mServerProperties.mServerName = DEFAULT_SERVER_NAME;
	}

	if (!mServerProperties.mServerPort)
	{
		Log("Unable to get 'ServerPort' from config file ! Using default value: '%d'", DEFAULT_SERVER_PORT);
		mServerProperties.mServerPort = DEFAULT_SERVER_PORT;
	}

	printf("Server Name: %s\n", mServerProperties.mServerName.c_str());
	printf("Server Pass: %s\n", (mServerProperties.mPassword.size()) ? mServerProperties.mPassword.c_str() : "<none>");
	printf("Server Port: %d\n", mServerProperties.mServerPort);
	printf("Server Max Players: %d\n", mServerProperties.mMaxPlayers);
	
	mNetwork = new Network();
	mNetwork->Init();

	mRunning = true;
}

void Core::Tick()
{
	mNetwork->Tick();
}

Core* Core::CoreInstance;

bool Core::IsRunning()
{
	return mRunning;
}

Core* Core::GetCore()
{
	return CoreInstance;
}
