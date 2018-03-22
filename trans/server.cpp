#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <ctime>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 4096
#define WIDTH 512
#define HEIGHT 424

DWORD b, e;

class Server {
public:
	Server() {}

	void Init() {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		memset(bufRecv, 0, sizeof(bufRecv));
		sock = socket(PF_INET, SOCK_STREAM, 0);
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = PF_INET;
		sockAddr.sin_addr.s_addr = inet_addr("192.168.1.10"); // TODO
		sockAddr.sin_port = htons(1235); // TODO
		bind(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		listen(sock, 20);
		offset = 0;
hash = 0;
	}
	void Clean() {
		WSACleanup();

	}
	void Receive() {
		int tot = 0, len;
			
		while (1) {
			int nsize = sizeof(SOCKADDR);
			csock=accept(sock, (SOCKADDR*)&clntAddr, &nsize);
			while (len = recv(csock, bufRecv, BUF_SIZE, 0)) {

				if (offset == 0 && tot == 0) b = GetTickCount();

				for(int i = 0; i < len; i++) {
					
					pic[offset++] = bufRecv[i];
hash = hash * 23 + bufRecv[i];
					if (offset == WIDTH * HEIGHT) {
						std::cerr << "Received " << tot << " pictures     Hash: " << hash << std::endl;
						tot += 1;
						offset = 0;
hash = 0;

					}
				}
				memset(bufRecv, 0, sizeof(bufRecv));
			}

			if (tot == 10000) break;
			closesocket(csock);
		}

	}

private:
	WSADATA wsaData;
	sockaddr_in sockAddr, clntAddr;
	char bufRecv[BUF_SIZE * 2];
	SOCKET sock, csock;
	char pic[WIDTH * HEIGHT * 2];
	int offset, hash;

};

Server server;
int main(){
freopen("recv.txt", "w",stdout);
	server.Init();
	server.Receive();
	e = GetTickCount();
std::cerr << e - b << std::endl;
	printf("%d\n", e - b);
	server.Clean();
}
