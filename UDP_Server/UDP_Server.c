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


	LOG_INFO("UDP Server...\n");
	
	WSADATA wsa;

	int iSocketServer;
	struct sockaddr_in tSocketServerAddr;

	int iSocketClient;
	struct sockaddr_in tSocketClientAddr;
	int iClientAddrLen;

	char acRecvBuff[1000];


	int msgCnt = 0;


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	//open socket
	iSocketServer = (int)socket(AF_INET, SOCK_DGRAM, 0);
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
	int iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));

	if(-1 == iRet)
	{
		LOG_INFO("Bind failed!\n");
		return -1;
	}



	while(1)
	{
		iClientAddrLen = sizeof(tSocketClientAddr);
		int iRecvlen = recvfrom(iSocketServer, acRecvBuff,999, 0, (struct sockaddr *)&tSocketClientAddr, &iClientAddrLen);
		if(iRecvlen >0)
		{
			acRecvBuff[iRecvlen] = '\0';
			char ip[20];
			inet_ntop(AF_INET, &tSocketClientAddr.sin_addr, ip,20);
			LOG_INFO("Received from %s:%s\n", ip,acRecvBuff);

			msgCnt++;

			snprintf(acRecvBuff, 1000, "Msg:%d", msgCnt);

			sendto(iSocketServer, acRecvBuff, strlen(acRecvBuff), 0, (struct sockaddr *)&tSocketClientAddr, iClientAddrLen);

			



		}

	}


	close(iSocketServer);



	


		
	return 0;
	
}