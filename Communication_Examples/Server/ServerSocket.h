#pragma once
#include "SocketBase.h"

class ServerSocket : public SocketBase
{
private:
    WSADATA wsaData;

    SOCKET ListenSocket;
    SOCKET ClientSocket;

    int CreateListener();
    int ListenToClients();

public:
    ServerSocket();
    static std::shared_ptr<ServerSocket> StartListening();

};

