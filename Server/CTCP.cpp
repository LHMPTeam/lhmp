// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include <sys/types.h>
#include <stdio.h>
#include "TCPInterface.h"
#define hStatus NULL

#ifndef _WIN32
#define Sleep usleep
#include <pthread.h>
pthread_t tid1;
#endif // _WIN32
extern CCore* g_CCore;

//void TCP(LPVOID v){
#ifdef _WIN32
void TCP(){
#else
void* TCP(void *arg){
#endif
	TCPInterface TCP;
	TCP.Start(g_CCore->GetNetworkManager()->GetPort(),10);
	RakNet::TimeMS t1 = RakNet::GetTimeMS();
	while (g_CCore->IsRunning())
	{
		if (RakNet::GetTimeMS() - t1 > 5000)
		{
			g_CCore->GetNetworkManager()->UpdateMasterlist();
			t1 = RakNet::GetTimeMS();
		}
		Sleep(100);
	}
}

CTCP::CTCP(int q)
{
	#ifdef _WIN32
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&TCP, 0, NULL, NULL);
	#else
	pthread_create(&tid1,NULL,&TCP,NULL);
	#endif
}


CTCP::~CTCP()
{
}
