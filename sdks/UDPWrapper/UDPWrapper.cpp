/**
    Lost Heaven Multiplayer
    UDPWrapper.cpp
	Purpose: simple UDP wrapper for LHMP written by me
	@author Romop5
*/

#include "UDPWrapper.h"
#include <stdio.h>

// starts UDP server at @PORT
// returns UDP_OK on success, or one of UDP enum errors
unsigned int UDPWrapper::StartServer(int port)
{
	struct sockaddr_in server;
	unsigned int result;

	// start WinSock 
	if ((result = this->StartSockets()) != UDP_OK)
		return result;

	//Create a socket
	if (this->CreateSocket() == INVALID_SOCKET)
		return UDP_SOCKETFAILED;

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Bind server's socket (to receive data on this port from remote)
	if (bind(p_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		closesocket(p_socket);
		return UDP_BINDINGFAILED;
	}

	// if everything went wrong, we are ready
	return UDP_OK;
}

// prepare client for communication with hostname:port
unsigned int UDPWrapper::StartClient(char* hostname, int port)
{
	unsigned int result;

	// start WinSock 
	if ((result = this->StartSockets()) != UDP_OK)
		return result;

	//Create a socket
	if (this->CreateSocket() == INVALID_SOCKET)
		return UDP_SOCKETFAILED;

	hostent* host = gethostbyname(hostname);
	char newhost[50] = "127.0.0.1";
	if (host)
	{
		strcpy(newhost, inet_ntoa(*(struct in_addr *)*host->h_addr_list));
	}

	//setup address structure
	memset((char *)&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
#ifdef _WIN32
	server.sin_addr.S_un.S_addr = inet_addr(newhost);
#else
	server.sin_addr.s_addr  =  inet_addr(newhost);
#endif
	return UDP_OK;
}

// send data to receiver - ONLY FOR CLIENT
void UDPWrapper::SendData(unsigned int length, const void* data)
{
	this->SendData(length, data, (struct sockaddr*) &this->server);
}

// send data to receiver
void UDPWrapper::SendData(unsigned int length, const void* data, sockaddr* receiver)
{
	int slen;
	slen = sizeof(this->server);
	if (sendto(this->p_socket, (const char*)data, length, 0, receiver, slen) == SOCKET_ERROR)
	{
		//printf("Shit, error %d\n", WSAGetLastError());

	}
}

// receive data
// if no data is available, returns NULL
UDPPacket* UDPWrapper::Receive()
{
	struct sockaddr_in si_other;
	socklen_t slen = sizeof(si_other);
	char buffer[64001];
	int recv_len;
	if ((recv_len = recvfrom(this->p_socket, buffer, 64000, 0, (struct sockaddr *) &si_other,&slen)) == SOCKET_ERROR)
	{
		return NULL;
	}
	
	if (recv_len > 0)
	{
		UDPPacket* packet = new UDPPacket;
		packet->data = new unsigned char[recv_len];
		packet->messageLength = recv_len;
		memcpy(&packet->sender, &si_other, sizeof(struct sockaddr_in));
		memcpy(packet->data, buffer, recv_len);
		return packet;
	}
	return NULL;
}

void UDPWrapper::CleanUP()
{
	
	
	// Unbind socket (if is binded)
#ifdef _WIN32
	BOOL optVal = true;
	const socklen_t optLen = sizeof(optVal);
	int rtn = setsockopt(this->p_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&optVal, optLen);
#else
	const int       optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	int rtn = setsockopt(this->p_socket, SOL_SOCKET, SO_REUSEADDR, (void*)&optVal, optLen);
#endif
	closesocket(this->p_socket);
}

//------------------------ PRIVATE ------------------------//

// starts sockets
unsigned int UDPWrapper::StartSockets()
{
#ifdef _WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return UDP_WINSOCKFAILED;
#endif
	return UDP_OK;
}
// create socket
SOCKET		UDPWrapper::CreateSocket()
{
	SOCKET sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		return sock;
	}
	this->p_socket = sock;
#ifdef _WIN32
	unsigned long iMode = 1;
	int iResult = ioctlsocket(sock, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}
#else
	int flags = fcntl(sock, F_GETFL, 0);
	if (flags < 0) return INVALID_SOCKET;
	flags = flags | O_NONBLOCK;
	if(fcntl(sock, F_SETFL, flags) != 0)
	{
		closesocket(sock);
		return INVALID_SOCKET;
	}
#endif
	return sock;
}
