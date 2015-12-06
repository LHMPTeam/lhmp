#include "CNetwork.h"
#include "CLocalPlayer.h"
#include "../shared/structures.h"
#include "../shared/CBitArray.h"
#include "CCore.h"
#include "RakNetTime.h"
#include <fstream>
#include "TCPInterface.h"
#include "../shared/version.h"

#include "FileListTransfer.h"
#include "CFileTransfer.h"
	  
extern CCore *g_CCore;

CNetworkManager::CNetworkManager()
{
	isConnected = false;
	peer = RakNet::RakPeerInterface::GetInstance();
	peer->Startup(1,&sd, 1);
	GetConnectInfo();
	ValidateIP();

	std::fstream nickname;
	
	/*nickname.open("lhmp/nickname.txt",std::ios::in);
	if(nickname.is_open())
	{
		nickname	>> NickName;
		nickname.close();
	}
	if(strlen(NickName) == 0)
		sprintf(NickName,"Unknown");
	*/
	// Loads values from Windows Registry system (e.g. nicknamestring)
	LoadRegistryConfig();
	//g_CCore->GetLocalPlayer()->SetNickname(NickName);


	transferPlugin = NULL;

}
CNetworkManager::~CNetworkManager()
{
	RakNet::RakPeerInterface::DestroyInstance(peer);
}

RakNet::RakPeerInterface* CNetworkManager::GetPeer()
{
	return peer;
}

bool CNetworkManager::ConnectServer()
{
	if (strlen(CONNECT_PASSWORD) > 0)
		peer->Connect(CONNECT_IP, CONNECT_PORT, CONNECT_PASSWORD, strlen(CONNECT_PASSWORD));
	else
		peer->Connect(CONNECT_IP,CONNECT_PORT,0,0);
	char buffer[255];
	sprintf(buffer,"Connecting %s:%d",CONNECT_IP,CONNECT_PORT);
	g_CCore->GetChat()->AddMessage(buffer);	
	return true;
}

void CNetworkManager::OnConnectionAccepted(RakNet::Packet* packet)
{
	g_CCore->GetChat()->AddMessage("#00d717Connection accepted.");
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_INITLHMP);
	//bsOut.Write(NickName);
	//bsOut.Write(g_CCore->GetGame()->CameraGetFov());
	bsOut.Write(LHMP_VERSION_TEST_HASH);
	peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void	CNetworkManager::OnConnectionIsAboutFinish()
{
	// send him CONNECTION FINAL
	isConnected = true;

	//g_CCore->GetChat()->AddMessage("#ff8600Almost there ...");

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID) ID_CONNECTION_FINISHED);
	bsOut.Write(NickName);
	bsOut.Write(g_CCore->GetGame()->CameraGetFov());
	peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void CNetworkManager::Pulse()
{
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
	{
		RakNet::TimeMS TS = NULL;
		int offset = 0;
		if (packet->data[0] == ID_TIMESTAMP)
		{
			// this packet has a timesteam before raw data
			RakNet::BitStream timestamp(packet->data+1, sizeof(RakNet::TimeMS)+1, false);
			timestamp.Read(TS);
			offset = 1 + sizeof(RakNet::TimeMS);
			
		}
		switch (packet->data[offset])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				this->OnConnectionAccepted(packet);
			}
			break;
		case ID_CONNECTION_FINISHED:
		{
				g_CCore->GetChat()->ClearChat();
				g_CCore->GetChat()->AddMessage("#ff8600Lost Heaven Multiplayer started.");
				char version[255];
#if LHMP_VERSION_TYPE == 1
				sprintf(version, "#fec606Version %d.%d", LHMP_VERSION_MAJOR,LHMP_VERSION_MINOR);
				g_CCore->GetChat()->AddMessage(version);
#else

				sprintf(version, "#ff8600Build hash: #e3e3e3%s", LHMP_VERSION_TEST_HASH);
				g_CCore->GetChat()->AddMessage(version);
				sprintf(version, "#ff8600Build time:  #e3e3e3%s (%s)", __DATE__, __TIME__);
				g_CCore->GetChat()->AddMessage(version);
#endif
				//g_CCore->GetChat()->AddMessage("#71ba51Connected to the server.");
				
				RakNet::BitStream bsIn(packet->data+offset, packet->length-offset, false);
				
				_Server svr;
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				bsIn.Read(svr);
				sprintf(this->m_pServerName, "%s", svr.server_name);
				g_CCore->GetLocalPlayer()->SetID(svr.playerid);

				char mapName[255];
				bsIn.Read(mapName);

				// TODO - change map if it differs

				g_CCore->GetEngineStack()->AddMessage(ES_CAMERAUNLOCK, 0);

				int len = strlen(mapName);
				char* vstup = new char[len + 1];
				strcpy(vstup, mapName);

				g_CCore->GetEngineStack()->AddMessage(ES_CHANGEMAP, (DWORD)vstup);

				// send him CONNECTION FINAL

				/*RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID) ID_CONNECTION_FINISHED);
				bsOut.Write(NickName);
				bsOut.Write(g_CCore->GetGame()->CameraGetFov());
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);*/

			}
			break;
		case ID_GAME_BAD_VERSION:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection aborted, the server is running a different version.");
				break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection aborted, the server is full.");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			g_CCore->GetChat()->AddMessage("#f31d2fDisconnected from the server.");
			break;
		case ID_CONNECTION_LOST:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection with the server lost.");
			break;
		case ID_CONNECTION_ATTEMPT_FAILED:
			g_CCore->GetChat()->AddMessage("#f31d2fConnection failed, trying to reconnect.");
			ConnectServer();
			break;
		case ID_INVALID_PASSWORD:
			g_CCore->GetChat()->AddMessage("#ff0000Password rejected.");
			break;
		case ID_CONNECTION_BANNED:
			g_CCore->GetChat()->AddMessage("#ff0000Connection refused - we are banned.");
			break;
		case ID_GAME_ALIVE:
		case ID_GAME_SYNC:
			break;
		case ID_GAME_LHMP_PACKET:
			{
				if (this->IsConnected())
					g_CCore->GetGameSync()->HandlePacket(packet,TS);
					//ProceedLHMP(packet,TS);
			}
			break;
		case ID_FILETRANSFER:
		{
			/*char buff[200];
			char foo[3];
			for (int i = 0; i < packet->length; i++)
			{
				sprintf(foo, "%2X", packet->data[i]);
				if (foo[1] == '0')
					foo[0] = '0';
				buff[i * 2] = foo[0];
				buff[1+i * 2] = foo[1];
			}
			buff[packet->length * 2] = 0x0;

			g_CCore->GetChat()->AddMessage(buff);
			*/

								if (packet->length > 2)
								{
									RakNet::BitStream bsIn(packet->data + 1, packet->length - 1, false);
									g_CCore->GetFileTransfer()->HandlePacket(&bsIn);
								}
								else {
									g_CCore->GetChat()->AddMessage("Fuck");
								}
		}
			break;

		case ID_SERVERRELOAD:
		{
			// replay connecting process
			this->isConnected = false;
			g_CCore->GetEngineStack()->AddMessage(ES_SERVERRELOAD, NULL);
			// now just wait what will come next
			g_CCore->GetFileSystem()->Reset();

			g_CCore->GetFileTransfer()->Reset();
			// Delete all current client scripts
			g_CCore->GetSquirrel()->DeleteScripts();
		}
			break;
		case ID_SCRIPSTLIST:
			RakNet::BitStream bsIn(packet->data + offset, packet->length - offset, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			unsigned short count = 0;
			char scriptname[256];
			bsIn.Read(count);
			for (int i = 0; i < count; i++)
			{
				bsIn.Read(scriptname);

				// try to start script
				//g_CCore->GetChat()->AddMessage(scriptname);
				g_CCore->GetSquirrel()->LoadClientScript(scriptname);
				//g_CCore->GetEngineStack()->AddMessage(ES_SCRIPT_RUN,NULL);
			}
			break;
		
		}
	}
}

void CNetworkManager::SendServerMessage(RakNet::BitStream* bsOut,PacketPriority PP = MEDIUM_PRIORITY,PacketReliability PR = RELIABLE_ORDERED)
{
	peer->Send(bsOut,PP,PR,0,RakNet::UNASSIGNED_RAKNET_GUID,true);
}

void CNetworkManager::GetConnectInfo()
{
	//char commandLine[255] = "\"C:\\nejaky string\" 50.50.50.50 127";
	//MessageBoxA(NULL,commandLine,"-1",MB_OK);
	char* commandLine	= GetCommandLine();
	LPCSTR pch;
	pch=strrchr(commandLine,'"');
	if(pch == NULL)
	{
		pch = strtok(commandLine," ");
		pch = strtok(NULL," ");
	} else {
		char buffer[255];
		pch = strtok(commandLine,"\"");
		//MessageBoxA(NULL,pch,"1",MB_OK);
		pch = strtok(NULL,"\"");
		//MessageBoxA(NULL,pch,"2",MB_OK);
		sprintf(buffer,"%s",pch);
		pch = strtok(buffer," ");
		//MessageBoxA(NULL,pch,"3",MB_OK);
	}

	// is there any IP as argument
	if(pch != NULL)
	{
		sprintf(CONNECT_IP,"%s",pch);
		pch = strtok(NULL," ");
		// is there any PORT as argument
		if(pch != NULL)
		{
			CONNECT_PORT = atoi(pch);
			pch = strtok(NULL, " ");
			if (pch != NULL)
			{
				// password argument
				sprintf(CONNECT_PASSWORD, "%s", pch);
			}
			else {
				sprintf(CONNECT_PASSWORD, "%s", "");
			}
		} else
		{
			CONNECT_PORT = 27015;
		}
	} else
	{
		// if there aren't any arguments, use default ones
		sprintf(CONNECT_IP,"%s","127.0.0.1");
		CONNECT_PORT = 27015;
	}
}

void CNetworkManager::ValidateIP()
{
	char buffer[255];
	sprintf(buffer,"%s",CONNECT_IP);
	if(strlen(buffer) < 7)
	{
		sprintf(CONNECT_IP,"%s","127.0.0.1");
	} else 
	{
		int countNum = 0;
		int countDot = 0;
		for(int i = 0; i < (int) strlen(buffer);i++)
		{
			if(buffer[i] == '.')
			{
				if(countNum == 0)
				{
					sprintf(CONNECT_IP,"%s","127.0.0.1");
					break;
				} else
				{
					countNum = 0;
					countDot++;
				}
			}
			else if(isdigit(buffer[i]))
			{
				countNum++;
			}
			else
			{
				sprintf(CONNECT_IP,"%s","127.0.0.1");
				break;
			}
		}
	}
}


char* CNetworkManager::GetNick()
{
	return NickName;
}

RakNet::RakPeerInterface* CNetworkManager::ReturnPeer()
{
	return peer;
}

bool CNetworkManager::IsConnected()
{
	return this->isConnected;
}

char*	CNetworkManager::GetServerName()
{
	return this->m_pServerName;
}
char* GetData(unsigned char* input, int size)
{
	char* newpole = new char[size + 1];
	memcpy(newpole, input, size);
	newpole[size] = 0x0;

	char* pointer = strstr(newpole, "\r\n\r\n");

	char* output = NULL;
	if (pointer != NULL)
	{
		//g_CCore->GetChat()->AddMessage("OK");
		pointer += 4;
		int len = size-(pointer - newpole);
		char* output = new char[len+1];
		memcpy(output, pointer, len);
		output[len] = 0x0;

		/*char ale[100];
		sprintf(ale, "%d %x", len, output);
		g_CCore->GetChat()->AddMessage(ale);*/
		return output;
	}
	else{
		//g_CCore->GetChat()->AddMessage("BAD");
		output = NULL;
	}
	//delete [] newpole;
	return output;

}
void httpRequestSlave(LPVOID pArgs)
{
	sHttpRequest* request = (sHttpRequest*)pArgs;
	RakNet::TCPInterface TCP;
	if (TCP.Start(0, 0) != false)
	{
		RakNet::SystemAddress server;
		char hostname[100];
		char rest[300];
		Tools::AnalyseUrl(request->buff, hostname, rest);
		server = TCP.Connect(hostname, 80, true);
		if (server != RakNet::UNASSIGNED_SYSTEM_ADDRESS)			// if we are connected
		{
			char postRequest[2048] = "";
			char data[1024] = "";
			//sprintf_s(data, "name=%s&pt=%d&ms=%d&mod=%s", svrname.c_str(), port, maxplayers, "");
			sprintf_s(postRequest,
				"GET %s HTTP/1.0\r\n"
				"Content-Type: application/x-www-form-urlencoded\r\n"
				"Content-Length: %d\r\n"
				//"Accept-Encoding: identity\r\n"
				"Host: %s\r\n\r\n", rest, strlen(data), hostname);
			//"%s\r\n", strlen(data), data);
			TCP.Send(postRequest, strlen(postRequest), server, false);
			RakNet::TimeMS StartTime = RakNet::GetTimeMS();
			while (1)
			{
				RakNet::Packet *packet = TCP.Receive();
				if (packet)
				{
				
					char* output = GetData(packet->data, packet->length);
					/*if (output == NULL)
					{
					} else {
						callback CB = (callback)request->callback;
						CB(output);
					}*/
					callback CB = (callback)request->callback;
					CB(output);
		
					break;
				}
				if (RakNet::GetTimeMS() - StartTime > 5000)
				{
					//g_CCore->GetChat()->AddMessage("Request failed");
					break;
				}
				Sleep(100);
			}
		}
		else
		{
			//g_CCore->GetChat()->AddMessage("Connection failed");
		}
	}
}
void CNetworkManager::httpRequest(int type, char* url, void* callback)
{
	sHttpRequest* request = new sHttpRequest();
	request->type = type;
	strcpy(request->buff, url);
	request->callback = callback;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&httpRequestSlave,request, 0, 0);
}


void CNetworkManager::LoadRegistryConfig()
{
	// load nickname
	HKEY key;
	HRESULT res = RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Lost Heaven Multiplayer\\Launcher"), &key);
	if (res != ERROR_SUCCESS)
	{
		strcpy(this->NickName, "UnknownPlayer");
	}
	else {
		DWORD size = 256;
		char nickname[256];
		DWORD type = REG_SZ;
		if (RegQueryValueEx(key, "nickname", NULL, &type, (LPBYTE)&nickname, &size) == ERROR_SUCCESS)
		{
			strcpy(this->NickName, nickname);
		}
		else {
			strcpy(this->NickName, "UnknownPlayer");
		}
		RegCloseKey(key);
	}

}