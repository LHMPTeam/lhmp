// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

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
    //usleep(ms);
    struct timespec tim, tim2;
    tim.tv_sec = 1;
    tim.tv_nsec = 100000;

    nanosleep(&tim , &tim2);
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


void CLHMPQuery::queryMasterlist(const char* IPaddress)
{
    this->AddTask((char*)IPaddress, 50000, TYPE_MASTERLIST,MASTERSERVER);
}


// intern function called by worker thread to ensure we are listening from server
void		CLHMPQuery::Tick()
{
    for (unsigned int i = 0; i < this->taskSize; i++)
    {
        unsigned int time = GetTickCount();
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
                    if (taskPool[i]->ID == MASTERSERVER)
                        this->OnMasterConnnectionFailed(i);
                    else
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
    case TYPE_MASTERLIST:
        client->SendData(5, "LHMPr");
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

void CLHMPQuery::OnMasterConnnectionFailed(unsigned int taskID)
{
    (*this->p_userCallback)(taskPool[taskID]->ID, NULL, QUERY_MASTER_FAILED);
    taskPool[taskID]->client->CleanUP();
    delete taskPool[taskID];
    taskPool[taskID] = NULL;
}

void CLHMPQuery::OnDataArrived(unsigned int taskID, char* data, unsigned int len)
{
    char signature[] = "LHMP";
    if (len > 5)
    {
        //if (data[0] == 'L' && data[1] == 'H' && data[2] == 'M' && data[3] == 'P')
        if (*(int*) data == *(int*) signature)
        {
            switch (data[4])
            {
            case 'o':
                this->ProcessOverall(taskID, data, len);
                break;
            case 'p':
                this->ProcessPlayerlist(taskID, data, len);
                break;
            case 'r':
                this->ProcessMaster(taskID, data, len);
                break;
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
    // 5 bytes stands for 'LHMPo'
    unsigned int dataRead = 5;

    // --- Read protocol version - initial version is 0
    unsigned char queryProtocol = data[dataRead];
    dataRead++;

    // this class was written for protocol 0, different protocol would cause incompability
    if (queryProtocol == 0)
    {
        // --- Read (bool) hasPassword - if == 1, server is protected with pass
        bool hasPassword = data[dataRead];
        dataRead++;

        // --- Read player's count / max players
        short players = *(short*)(data + dataRead);
        dataRead += 2;
        short maxPlayers = *(short*)(data + dataRead);
        dataRead += 2;

        // --- Read server name
        int serverNameLen = data[dataRead];
        dataRead++;

        char* serverName = new char[serverNameLen + 1];
        memcpy(serverName, data + dataRead, serverNameLen);
        serverName[serverNameLen] = 0x0;
        dataRead += serverNameLen;

        // --- Read server mode
        int serverModeLen = data[dataRead];
        dataRead++;

        char* serverMode = new char[serverModeLen + 1];
        memcpy(serverMode, data + dataRead, serverModeLen);
        serverMode[serverModeLen] = 0x0;
        dataRead += serverModeLen;

        // --- Read server website
        int serverWebLen = data[dataRead];
        dataRead++;

        char* serverWeb = new char[serverWebLen + 1];
        memcpy(serverWeb, data + dataRead, serverWebLen);
        serverWeb[serverWebLen] = 0x0;
        dataRead += serverWebLen;

        // --- Read server map
        int serverMapLen = data[dataRead];
        dataRead++;

        char* serverMap = new char[serverMapLen + 1];
        memcpy(serverMap, data + dataRead, serverMapLen);
        serverMap[serverMapLen] = 0x0;
        dataRead += serverMapLen;

        // Process given data
        unsigned int ping = GetTickCount() - taskPool[taskID]->timeStamp;
        OverallPacket* packet = new OverallPacket(taskPool[taskID]->ID, serverName, serverMode, players, maxPlayers, ping, serverMap,serverWeb,hasPassword);
        (*this->p_userCallback)(taskPool[taskID]->ID, packet, (unsigned char)QUERY_OVERALL);
        delete packet;
    }
    taskPool[taskID]->client->CleanUP();
    delete taskPool[taskID];
    taskPool[taskID] = NULL;
}

void CLHMPQuery::ProcessPlayerlist(unsigned int taskID, char* data, unsigned  int len)
{
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

        //printf("Player[%d]: %s\n", playerID, playerString);
        // place info about player into packet
        packet->playersPool[i].ID = playerID;
        strcpy(packet->playersPool[i].nickname, playerString);
    }

    (*this->p_userCallback)(taskPool[taskID]->ID, packet, (unsigned char)QUERY_PLAYERLIST);
    delete packet;
    delete taskPool[taskID];
    taskPool[taskID] = NULL;

}


void CLHMPQuery::ProcessMaster(unsigned int taskID, char* data, unsigned int len)
{
    unsigned short count = *(short*)(data + 5);
    //printf("Master response, count: %u \n",count);
    MasterResponse* packet = new MasterResponse(count);
    for (int i = 0; i < count; i++)
    {
        // 6 - size of item (4bytes IP, 2bytes port)
        in_addr serverIP = *(in_addr*)(data + (i * 6) + 7);
        unsigned short port = *(unsigned short*)(data + (i * 6) + 11);
        //printf("Server %d: %s:%u \n", i, inet_ntoa(serverIP), port);
        packet->servers[i] = Server(inet_ntoa(serverIP), port);
    }

    // Clean up task
    //printf("co to kurva\n");
    (*this->p_userCallback)(taskPool[taskID]->ID, packet, (unsigned char)QUERY_MASTER_SUCCESS);
    delete packet;
    taskPool[taskID]->client->CleanUP();
    delete taskPool[taskID];
    taskPool[taskID] = NULL;

    //printf("co to kurva %d %p\n", taskID, taskPool[taskID]);
}
