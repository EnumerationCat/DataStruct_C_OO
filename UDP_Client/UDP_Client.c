#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>



#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#define SERVER_PORT 8888
#define BACKLOG 10


#define LOG_INFO(fmt, ...)  printf("%s:%s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)


/*UDP_Client  127.0.0.1*/
int main(int argc, char **argv) {

	if(argc != 2)
	{
		LOG_INFO("Usage: %s <IP Address>\n", argv[0]);
		return -1;
	}


	LOG_INFO("UDP Client...\n");
	
	WSADATA wsa;

	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	char acSendBuff[1000];


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	//open socket
	iSocketClient = (int)socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == iSocketClient)
	{

		LOG_INFO("Socket creation failed!\n");
		return -1;
	}


	//bind socket
	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	int res = inet_pton(tSocketServerAddr.sin_family, argv[1], &tSocketServerAddr.sin_addr);
	if(res <= 0)
	{
		LOG_INFO("Invalid server_ip\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);
	while(1)
	{
		int iAddrLen = sizeof(tSocketServerAddr);
		if(fgets(acSendBuff,999, stdin))
		{
			int iSendLen = sendto(iSocketClient, acSendBuff, strlen(acSendBuff), 0,
			 					 (const struct sockaddr *)&tSocketServerAddr, 
			 					  iAddrLen);
		}


	}

	close(iSocketClient);
	return 0;
	
}