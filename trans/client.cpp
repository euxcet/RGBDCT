#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 100


int main(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family=PF_INET;
	sockAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sockAddr.sin_port=htons(1234);

	char bufSend[BUF_SIZE]={0};
	char bufRecv[BUF_SIZE]={0};
	while(1){
		SOCKET sock=socket(PF_INET,SOCK_STREAM,0);
		connect(sock,(SOCKADDR*)&sockAddr,sizeof(SOCKADDR));

		printf("Input a string:");
		gets(bufSend);
		send(sock,bufSend,strlen(bufSend),0);

		recv(sock,bufRecv,BUF_SIZE,0);
		
		if(bufRecv[0]=='q')
			break;
		printf("Message from server:%s\n",bufRecv);

		memset(bufSend,0,BUF_SIZE);
		memset(bufRecv,0,BUF_SIZE);
		closesocket(sock);
	}

	WSACleanup();

	system("pause");
	return 0;

}
