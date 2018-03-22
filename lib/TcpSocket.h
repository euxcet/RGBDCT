#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <WinSock2.h>
#include <omp.h>
#pragma comment(lib,"ws2_32.lib")
#define BUF_SIZE 4096
#define HBUF_SIZE 2048
#define WIDTH 512
#define HEIGHT 424


class TcpSocket {
public:
	TcpSocket() { offset = 0; }
	virtual void Init(const char* ip, int port) = 0;
	virtual void Run() = 0;

	void Clean() { WSACleanup(); }
	void SendDepthData();
	void SendRGBData();

	void Receive();

	void SetDepthData(const UINT16* data, int len);
	void SetRGBData(const RGBQUAD* data, int len);

protected:
	WSADATA wsaData;
	sockaddr_in sockAddr;
	SOCKET sock;

	char bufSend[BUF_SIZE];
	char bufRecv[BUF_SIZE];
	char pic[WIDTH * HEIGHT];

	UINT16* depthData;
	RGBQUAD* rgbData;
	int depthLen;
	int rgbLen;
	int offset;
};

class TcpClient : public TcpSocket {
public:
	TcpClient() {}
	void Init(const char* ip, int port) {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = PF_INET;
		sockAddr.sin_addr.s_addr = inet_addr(ip);
		sockAddr.sin_port = htons(port);
		memset(bufSend, 0, sizeof(bufSend));
	}

	void Run() {
		Init("127.0.0.1", 1234);
		while (true) {
//#pragma omp critical
			{
				// TODO send data while receiving.
				if (rgbLen > 0)
					SendRGBData();
				if (depthLen > 0)
					SendDepthData();
				Receive();
			}
		}
		Clean();
	}

};


class TcpServer : public TcpSocket {
public:
	TcpServer() {}
	void Init(const char* ip, int port) {
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		memset(bufRecv, 0, sizeof(bufRecv));
		sock = socket(PF_INET, SOCK_STREAM, 0);
		memset(&sockAddr, 0, sizeof(sockAddr));
		sockAddr.sin_family = PF_INET;
		sockAddr.sin_addr.s_addr = inet_addr(ip);
		sockAddr.sin_port = htons(port);
		bind(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		listen(sock, 20);
	}

	void Run() {
		Init("127.0.0.1", 1234);
		while (true) {
//#pragma omp critical
			{
				// TODO send data while receiving.
				if (rgbLen > 0)
					SendRGBData();
				if (depthLen > 0)
					SendDepthData();
				Receive();
			}
		}
		Clean();
	}
};


#endif
