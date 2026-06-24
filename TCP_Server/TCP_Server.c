#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>



#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#define SERVER_PORT 8888
#define BACKLOG 10


#define LOG_INFO(fmt, ...)  printf("%s:%s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)



int main() {


	LOG_INFO("Hello, World!\n");
	WSADATA wsa;

	int iSocketServer;
	struct sockaddr_in tSocketServerAddr;

	int iSocketClient;
	struct sockaddr_in tSocketClientAddr;
	int iClientAddrLen;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	//open socket
	iSocketServer = (int)socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == iSocketServer)
	{

		LOG_INFO("Socket creation failed!\n");
		return -1;
	}


	//bind socket
	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);
	bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));


	//listen socket
	listen(iSocketServer, BACKLOG);
	int iClientNum = 0;
	int MessageCnt = 0;

	


	while (1)
	{
		iClientAddrLen = sizeof(tSocketClientAddr);
		iSocketClient = (int)accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iClientAddrLen);
		if(-1 != iSocketClient)
		{
			char ip[20];
			inet_ntop(AF_INET, &tSocketClientAddr.sin_addr, ip, 20);
			LOG_INFO("Client connected: %s:%d\n", ip, iClientNum++);

			while (1)
			{
				int iRecvLen;
				char ucRecvBuffer[1024];

				iRecvLen = recv(iSocketClient, (char*)ucRecvBuffer,999, 0);

				if(iRecvLen > 0)
				{
					MessageCnt++;
					ucRecvBuffer[iRecvLen] = '\0';
					LOG_INFO("Received from client%d: %s\n", iClientNum , ucRecvBuffer);


					sprintf(ucRecvBuffer, "Message %d received", MessageCnt);
					send(iSocketClient, ucRecvBuffer, strlen(ucRecvBuffer), 0);
				}else{
					close(iSocketClient);
					LOG_INFO("Client%d disconnected.\n", iClientNum);
					break;
				}
			}
			

		}

		
	}
	return 0;
	
}