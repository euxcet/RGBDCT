#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 100

int main(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);

	SOCKET servSock=socket(PF_INET,SOCK_STREAM,0);


	sockaddr_in sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));
	sockAddr.sin_family=PF_INET;
	sockAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sockAddr.sin_port=htons(1234);
	bind(servSock,(SOCKADDR*)&sockAddr,sizeof(SOCKADDR));

	listen(servSock,20);

	SOCKADDR clntAddr;
	int nsize=sizeof(SOCKADDR);
	char buffer[BUF_SIZE];
	while(1){
		SOCKET clntSock=accept(servSock,(SOCKADDR*)&clntAddr,&nsize);
		int strLen=recv(clntSock,buffer,BUF_SIZE,0);
		printf("%s\n",buffer);
		send(clntSock,buffer,strLen,0);

		closesocket(clntSock);
		if(buffer[0]=='q')
			break;
		memset(buffer,0,BUF_SIZE);

	}
	closesocket(servSock);
	WSACleanup();
	return 0;

}
