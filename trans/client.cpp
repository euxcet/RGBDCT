#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <WinSock2.h>
#include <fstream>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
#define WIDTH 512
#define HEIGHT 424

DWORD b, e;

class Client {
public:
	Client() {}

	void Init() {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = PF_INET;
		sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // TODO
		sockAddr.sin_port = htons(1234); // TODO
		memset(bufSend, 0, sizeof(bufSend));
	}
	void Clean() {
		WSACleanup();

	}
	void Send(const char* str, int length) {
		sock = socket(PF_INET, SOCK_STREAM, 0);
		connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		send(sock, str, length, 0);
		closesocket(sock);
	}

private:
	WSADATA wsaData;
	sockaddr_in sockAddr;
	char bufSend[BUF_SIZE];


	SOCKET sock;

};

Client client;

char s[WIDTH * HEIGHT];
char se[16];

int main(){
	client.Init();

	b = GetTickCount();

	// Generate a 512*424 RGB Image
	for(int i = 0; i < WIDTH * HEIGHT; i++) s[i] = rand() % 256 - 128;

	// Send 1000 times
	for(int p = 0; p < 1000; p++) {
		client.Send(s, WIDTH * HEIGHT);
	}

	e = GetTickCount();
	printf("%d\n", e - b);
	client.Clean();
}
