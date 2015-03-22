#include <stdio.h>
#include "UDPWrapper.h"

#define welcomeMessage "This is server, biatch !"

// Simple server
#pragma comment(lib,"ws2_32.lib")

void DumpHex(unsigned char* data, int length)
{
	for (int i = 0; i < length; i++)
		printf("%x %c ", data[i], data[i]);
	printf("\n");
}

int main()
{
	UDPWrapper* server = new UDPWrapper();
	UDPWrapper* client = new UDPWrapper();
	unsigned int result;
	result = server->StartServer(8888);
	if (result == UDP_OK)
	{
		printf("Server started ...\n");
		client->StartClient("127.0.0.1",8888);
		char messate[] = "Mamma";
		client->SendData(strlen(messate), messate);
		for (;;)
		{
			UDPPacket* pack;
			// if there is a new packet
			if ((pack = server->Receive()) != NULL)
			{
				DumpHex(pack->data, pack->messageLength);
				server->SendData((strlen(welcomeMessage) + 1), welcomeMessage,(sockaddr*) &pack->sender);
			}
		}
		return 0;
	}
	else {
		printf("Server couldnt started correctly\n");
		printf("Error: %u\n", result);
	}
	getchar();
	getchar();
}