#pragma once

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class SocketBase
{
private:
	WSADATA wsaData;
	struct addrinfo hints;
	struct addrinfo* addressInfo;

	int ResolveAddress(const char* ipAddress);

protected: 
	const int DEFAULT_BUFFER_LENGTH = 1024;
	const char* DEFAULT_PORT = "50000";

	SocketBase();

	int Initialize(int addressInfoFlag);
	
	void Connect();
	void Disconnect();
	
	int SendData(SOCKET remoteSocket, const char* data);
	int ReceiveData(SOCKET remoteSocket, char* data);
};

