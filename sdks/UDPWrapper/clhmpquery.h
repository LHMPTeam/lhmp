#ifndef _CLHMPQUERY_H
#define _CLHMPQUERY_H

#ifdef _WIN32
//#include <winsock2.h>
#else
#include <string.h>
#define sprintf_s sprintf
#endif
#include <stdio.h>

#include "UDPWrapper.h"

typedef void(*callbackfunction)(unsigned int,void*, unsigned char);

struct Player
{
	int ID;
	char nickname[255];
};

struct PlayerPacket {
	int ID;
	int playerCount;
	Player* playersPool;
	PlayerPacket(int inID,int count)
	{
		ID = inID;
		playerCount = count;
		playersPool = new Player[count];
	}
};

struct OverallPacket{
    int ID;
    char hostname[255];
    char gamemode[255];
	char mapname[255];
    short players, maxPlayer;
	unsigned int ping;
    OverallPacket(int nID, char* host, char* mode, short pl, short max,unsigned int INping,char* INmapname)
    {
        ID = nID;
        strcpy(hostname, host);
		strcpy(gamemode, mode);
        players = pl;
        maxPlayer = max;
		ping = INping;
		strcpy(mapname, INmapname);
    }
};

enum CallbackEvent
{
    QUERY_FAILED,
    QUERY_CONNECTION_FAILED,
    QUERY_OVERALL,
    QUERY_PLAYERLIST
};
enum Types
{
    TYPE_BASIC,
    TYPE_PLAYERLIST
};
enum TaskStates
{
    TASK_WAITINGFORCONNECTIONDONE,
    TASK_WAITINGFORDATA
};
struct sTask
{
	UDPWrapper* client;
    unsigned int ID;
    unsigned char queryType;
    unsigned int timeStamp;
	sTask(UDPWrapper* inClient, unsigned int inID, unsigned char inQueryType, unsigned int inTimestamp)
    {
		client = inClient;
		ID = inID;
		queryType = inQueryType;
		timeStamp = inTimestamp;
    }
};
class CLHMPQuery
{
public:
    static CLHMPQuery* getInstance()
    {
        if (!p_Instance)
        {
            p_Instance = new CLHMPQuery;
        }
        return p_Instance;
    }

    // Prepare query system -> start Winsock, start a new thread, etc.
    bool Prepare(void* callback, unsigned int timeout = 1000);


    // Query IP:port to get elementary information (players count, slots, hostname)
    // You can specify unicate ID
    void queryInfo(const char* IPaddress, unsigned int port, unsigned int ID = (unsigned int)-1);
    // Query IP:port to get player list
    // You can specify unicate ID
	void queryPlayerlist(const char* IPaddress, unsigned int port, unsigned int ID = (unsigned int)-1);
    // Query IP:port to get elementary + playerlist

    // intern function called by worker thread to ensure we are listening from server
    void			Tick();

private:
    static CLHMPQuery* p_Instance;
    CLHMPQuery() {};
    // user-defined callback which is called when an event happens (data about server arrives or timeout)
    callbackfunction	p_userCallback;

    bool			AddTask(char* IP, int port, unsigned char type, unsigned int ID);

    unsigned int	taskSize;
    sTask* taskPool[1000];

    void OnConnnectionFailed(unsigned int taskID);
    void OnDataArrived(unsigned int taskID, char* data, unsigned int len);

    void ProcessOverall(unsigned int taskID, char* data, unsigned int len);
    void ProcessPlayerlist(unsigned int taskID, char* data, unsigned int len);

};

#endif