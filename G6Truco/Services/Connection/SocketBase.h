#pragma once

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Exceptions\BufferOverflowException.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


namespace G6Truco::Services::Connection
{
	class SocketBase
	{
	private:
		WSADATA wsaData;
		struct addrinfo hints;
		struct addrinfo* addressInfo;

		SOCKET commSocket;

		int InitSocket(const char* ipAddress);
		int ResolveAddress(const char* ipAddress);

	protected:
		const int DEFAULT_BUFFER_LENGTH = 1024;
		const char* DEFAULT_PORT = "50000";

		SocketBase();

		int Bind();
		int Listen();
		int Accept();
		int Initialize(const char* ipAddress, int addressInfoFlag = NULL);

		int Connect();
		void Disconnect();

		int SendData(const char* data);
		int ReceiveData(char* data);
	};
}

