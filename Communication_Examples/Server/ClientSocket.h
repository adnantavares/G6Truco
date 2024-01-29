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

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class ClientSocket
{
private:
    WSADATA wsaData;

    SOCKET ConnectSocket;

    struct addrinfo* addressInfo, * ptr, hints;

    char recvbuf[DEFAULT_BUFLEN];
    
    int recvbuflen;

    int Initialize();
    int ResolveAddress();
    int Connect();
    int ListenToClients();

public:
    ClientSocket();
    static std::shared_ptr<ClientSocket> StartListening();

};

