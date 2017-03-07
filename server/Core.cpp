#include <stdinc.h>

Core::Core() : 
	mTickManager(nullptr),
	mNetwork(nullptr)
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
	printf("[SERVER] [%s] - %s\n", Utils::currentDateTime().c_str(), message);
}

void Core::LogW(const wchar_t* format, ...)
{
	va_list ap;
	wchar_t message[1024] = { 0 };
	va_start(ap, format);
	vswprintf(message, format, ap);
	va_end(ap);

	std::string toConvert = Utils::currentDateTime();
	wprintf(L"[SERVER] [%s] - %s\n", std::wstring(toConvert.begin(), toConvert.end()).c_str(), message);
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

	Log("Server Name: %s", mServerProperties.mServerName.c_str());
	Log("Server Pass: %s", (mServerProperties.mPassword.size()) ? mServerProperties.mPassword.c_str() : "<none>");
	Log("Server Port: %d", mServerProperties.mServerPort);
	Log("Server Max Players: %d\n", mServerProperties.mMaxPlayers);
	
	mTickManager = new TickManager();
	mNetwork = new Network();
	mNetwork->Init();
	mRunning = true;
}

void Core::Tick()
{
	mTickManager->GameTick();
	Core::GetCore()->GetNetwork()->Tick();	
}

Core* Core::CoreInstance;

Core* Core::GetCore()
{
	return CoreInstance;
}