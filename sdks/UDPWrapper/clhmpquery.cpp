#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <time.h>
#include <pthread.h>
pthread_t tid;
void Sleep(unsigned int ms)
{
	usleep(ms);
}

unsigned int GetTickCount()
{
	struct timespec ts;
	unsigned theTick = 0U;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	return theTick;
}
#endif
#include <stdio.h>

#include "CLHMPQuery.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

CLHMPQuery* CLHMPQuery::p_Instance = NULL;

#ifdef _WIN32
void threadCallback()
#else
void* threadCallback(void *arg)
#endif
{
    CLHMPQuery* query = CLHMPQuery::getInstance();
    while (1 == 1)
    {
        query->Tick();
        Sleep(5);
    }
}

void CLHMPQuery::queryInfo(const char* IPaddress, unsigned int port, unsigned int ID)
{
    this->AddTask((char*)IPaddress, port, TYPE_BASIC, ID);
}

void CLHMPQuery::queryPlayerlist(const char* IPaddress, unsigned int port, unsigned int ID)
{
    this->AddTask((char*)IPaddress, port, TYPE_PLAYERLIST, ID);
}

// intern function called by worker thread to ensure we are listening from server
void		CLHMPQuery::Tick()
{
    for (unsigned int i = 0; i < this->taskSize; i++)
    {
		unsigned int time = GetTickCount();
        //printf("suve!\n");
        if (taskPool[i] != NULL)
        {
			UDPPacket* pack = taskPool[i]->client->Receive();
			if (pack)
			{
				this->OnDataArrived(i, (char*) pack->data, pack->messageLength);
			} else {
				// if timeout
				if ((time - taskPool[i]->timeStamp) > 1000)
				{
					this->OnConnnectionFailed(i);
				}
			}
		}
    }
}

bool CLHMPQuery::Prepare(void* callback, unsigned int timeout)
{
    // Allocate memory for tasks and zero it
    taskSize = 1000;
    for (unsigned int i = 0; i < taskSize; i++)
        taskPool[i] = NULL;

    this->p_userCallback = (callbackfunction) callback;

#ifdef _WIN32
    if (CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&threadCallback, 0, NULL, NULL) == NULL)
        return false;
#else
	if (pthread_create(&tid, NULL, &threadCallback, NULL) != 0)
		return false;
#endif

    return true;
}

bool	CLHMPQuery::AddTask(char* IP, int port, unsigned char type, unsigned int ID)
{
	unsigned int freeTask = (unsigned int)-1;
	for (unsigned int i = 0; i < taskSize; i++)
	{
		if (taskPool[i] == NULL)
		{
			freeTask = i;
			break;
		}
	}

	// if there is no free slot available
	if (freeTask == (unsigned int)-1)
		return false;

	UDPWrapper* client = new UDPWrapper();
	client->StartClient(IP, port);
	switch (type)
	{
	case TYPE_BASIC:
		client->SendData(5,"LHMPo");
		break;
	case TYPE_PLAYERLIST:
		client->SendData(5,"LHMPp");
		break;
	default:
		return false;
		break;
	}

	taskPool[freeTask] = new sTask(client, ID, NULL, GetTickCount());
    return true;
}

void CLHMPQuery::OnConnnectionFailed(unsigned int taskID)
{
    (*this->p_userCallback)(taskPool[taskID]->ID,NULL, QUERY_CONNECTION_FAILED);
	taskPool[taskID]->client->CleanUP();
    delete taskPool[taskID];
    taskPool[taskID] = NULL;
}
void CLHMPQuery::OnDataArrived(unsigned int taskID, char* data, unsigned int len)
{
    if (len > 5)
    {
        if (data[0] == 'L' && data[1] == 'H' && data[2] == 'M' && data[3] == 'P')
        {
            switch (data[4])
            {
            case 'o':
                this->ProcessOverall(taskID, data, len);
                break;
            case 'p':
                this->ProcessPlayerlist(taskID, data, len);
            }
            return;
        }
    }

	(*this->p_userCallback)(taskPool[taskID]->ID, NULL, QUERY_FAILED);
    delete taskPool[taskID];
    taskPool[taskID] = NULL;
}

void CLHMPQuery::ProcessOverall(unsigned int taskID, char* data, unsigned int len)
{
	/*for (int i = 0; i < len; i++)
	{
		printf("%2X%c ", (unsigned char)data[i], data[i]);
	}
	printf("\n");
	*/

    int serverNameLen = data[5];
    char* serverName = new char[serverNameLen + 1];
    memcpy(serverName, data + 6, serverNameLen);
    serverName[serverNameLen] = 0x0;
    //printf("ServerName: '%s'\n", serverName);

    int serverModeLen = data[6 + serverNameLen];
    char* serverMode = new char[serverModeLen + 1];
    memcpy(serverMode, data + 7 + serverNameLen, serverModeLen);
    serverMode[serverModeLen] = 0x0;
    //printf("ServerMode: '%s'\n", serverMode);

    short players = *(short*)(data + 7 + serverNameLen + serverModeLen);
    short maxPlayers = *(short*)(data + 9 + serverNameLen + serverModeLen);
    //printf("ServerPlayers: '%u/%u'\n", players, maxPlayers);

	int serverMapLen = (int) *(unsigned char*)(data + 11 + serverNameLen + serverModeLen);
	char* serverMap = new char[serverMapLen + 1];
	memcpy(serverMap, data + 12 + serverNameLen + serverModeLen, serverMapLen);
	serverMap[serverMapLen] = 0x0;

	//printf("MapName: %d '%s\n", serverMapLen,serverMap);

	unsigned int ping = GetTickCount()-taskPool[taskID]->timeStamp;
	OverallPacket* packet = new OverallPacket(taskPool[taskID]->ID, serverName, serverMode, players, maxPlayers, ping, serverMap);
	(*this->p_userCallback)(taskPool[taskID]->ID, packet, (unsigned char)QUERY_OVERALL);
    delete packet;
	taskPool[taskID]->client->CleanUP();
    delete taskPool[taskID];
    taskPool[taskID] = NULL;
}

void CLHMPQuery::ProcessPlayerlist(unsigned int taskID, char* data, unsigned  int len)
{
    /*for (int i = 0; i < len; i++)
    {
        //printf("%2X%c ", (unsigned char)data[i], data[i]);
    }*/

    int alreadyUsed = 7;
    int playerID, stringLen;
    char playerString[256];
    short playersCount = *(unsigned short*)(data + 5);
	// initialize empty PlayerPacket for 'playercount' players
	PlayerPacket* packet = new PlayerPacket(taskPool[taskID]->ID,playersCount);

    //printf("Player count: %d \n", playersCount);
    for (int i = 0; i < playersCount; i++)
    {
        playerID = *(unsigned short*)(data + alreadyUsed);
        stringLen = *(unsigned char*)(data + alreadyUsed + 2);
        memcpy(playerString, data + (alreadyUsed + 3), stringLen);
        playerString[stringLen] = 0x0;	// fill the missing \0 terminator
        alreadyUsed += 3 + stringLen;

        printf("Player[%d]: %s\n", playerID, playerString);
		// place info about player into packet
		packet->playersPool[i].ID = playerID;
		strcpy(packet->playersPool[i].nickname, playerString);
    }

	(*this->p_userCallback)(taskPool[taskID]->ID, packet, (unsigned char)QUERY_PLAYERLIST);
	delete packet;
	delete taskPool[taskID];
	taskPool[taskID] = NULL;
 
}
