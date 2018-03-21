#include "TcpSocket.h"

void TcpSocket::SendDepthData() {
	sock = socket(PF_INET, SOCK_STREAM, 0);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    for(int i = 0; i < depthLen; i += HBUF_SIZE) {
		int bs = HBUF_SIZE < (depthLen - i) ? HBUF_SIZE : (depthLen - i);
		for(int j = 0; j < bs; j++) {
			bufSend[j << 1] = (char)(depthData[i + j] >> 8);
			bufSend[j << 1 | 1] = (char)(depthData[i + j] & 255);
		}
		send(sock, bufSend, bs, 0);
	}
	closesocket(sock);
}

void TcpSocket::SendRGBData() {
	sock = socket(PF_INET, SOCK_STREAM, 0);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    for(int i = 0; i < rgbLen; i += BUF_SIZE) {
		int bs = BUF_SIZE < (rgbLen - i) ? BUF_SIZE : (rgbLen - i);
		for(int j = 0; j < bs; j++)
			bufSend[j] = (char)rgbData[i + j].rgbRed;
		send(sock, bufSend, bs, 0);
		for(int j = 0; j < bs; j++)
			bufSend[j] = (char)rgbData[i + j].rgbGreen;
		send(sock, bufSend, bs, 0);
		for(int j = 0; j < bs; j++)
			bufSend[j] = (char)rgbData[i + j].rgbBlue;
		send(sock, bufSend, bs, 0);
	}
	closesocket(sock);
}

void TcpSocket::Receive() {
	int nsize = sizeof(SOCKADDR);
	int len;
	csock = accept(sock, (SOCKADDR*)&clntAddr, &nsize);
	while (len = recv(csock, bufRecv, BUF_SIZE, 0)) {
		for(int i = 0; i < len; i++) {
			pic[offset + i] = bufRecv[i];
		}
		offset += len;
	}
	closesocket(csock);
}

void TcpSocket::SetDepthData(const UINT16* data, int len) {
	#pragma omp critical
	{
		depthLen = len;
		for(int i = 0; i < len; i++)
			depthData[i] = data[i];
	}
}
void TcpSocket::SetRGBData(const RGBQUAD* data, int len) {
	#pragma omp critical
	{
		rgbLen = len;
		for(int i = 0; i < len; i++)
			rgbData[i] = data[i];
	}
}
