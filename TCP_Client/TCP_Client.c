#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>



#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#define SERVER_PORT 8888
#define BACKLOG 10


#define LOG_INFO(fmt, ...)  printf("%s:%s:%d: " fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

/*argv[0]         argv[1]*/
/*Socket_Client 127.0.0.1*/
int main(int argc, char**argv) {

	if(argc != 2)
	{
		LOG_INFO("Usage: %s <server_ip>\r\n", argv[0]);
		return -1;
	}

	LOG_INFO("Hello, World!\n");
	WSADATA wsa;


	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);


	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
	int res = inet_pton(tSocketServerAddr.sin_family,argv[1], &(tSocketServerAddr.sin_addr));

	if(res <= 0)
	{
		LOG_INFO("invalid server_ip\n");
		return -1;
	}


	memset(tSocketServerAddr.sin_zero, 0, 8);


	int iRet = connect(iSocketClient, (struct sockaddr*)&tSocketServerAddr, sizeof(struct sockaddr));

	if(-1 == iRet)
	{
		LOG_INFO("connect failed\n");
		return -1;
	}



	while(1)
	{
		char ucSendBuf[1000];
		if(fgets((char*)ucSendBuf, 999, stdin))
		{
			send(iSocketClient, (char*)ucSendBuf, (int)strlen((char*)ucSendBuf), 0);
		}
		else
		{
			LOG_INFO("fgetc failed\n");
			close(iSocketClient);
			return -1;
		}
	}

	return 0;
	
}