#include "ServerSocket.h"


ServerSocket::ServerSocket()
{
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;
}


int ServerSocket::CreateListener()
{
    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) 
    {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addressInfo);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    int result = bind(ListenSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

    if (result == SOCKET_ERROR) 
    {
        std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
        freeaddrinfo(addressInfo);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(addressInfo);
}

int ServerSocket::ListenToClients()
{
    int result = listen(ListenSocket, SOMAXCONN);

    if (result == SOCKET_ERROR) 
    {
        std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);

    if (ClientSocket == INVALID_SOCKET) 
    {
        std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // No longer need server socket
    closesocket(ListenSocket);
}

std::shared_ptr<ServerSocket> ServerSocket::StartListening()
{
    std::shared_ptr<ServerSocket> serverSocket = std::make_shared<ServerSocket>();

    serverSocket->Initialize(AI_PASSIVE);
    serverSocket->ResolveAddress();
    serverSocket->CreateListener();
    serverSocket->ListenToClients();

    return serverSocket;
}
