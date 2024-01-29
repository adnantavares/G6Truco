#include "ClientSocket.h"


ClientSocket::ClientSocket()
{
    ConnectSocket = INVALID_SOCKET;

    addressInfo = NULL;
    ptr = NULL;

    recvbuflen = DEFAULT_BUFLEN;
}

int ClientSocket::Initialize()
{
    // Initialize Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
}

int ClientSocket::ResolveAddress()
{
    // Resolve the server address and port
    int result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &addressInfo);

    if (result != 0)
    {
        std::cout << "getaddrinfo failed with error: " << result << std::endl;
        WSACleanup();
        return 1;
    }
}

int ClientSocket::Connect()
{
    // Attempt to connect to an address until one succeeds
    for (ptr = addressInfo; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        int result = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        
        if (result == SOCKET_ERROR) 
        {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(addressInfo);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
}

std::shared_ptr<ClientSocket> ClientSocket::StartListening()
{
    std::shared_ptr<ClientSocket> serverSocket = std::make_shared<ClientSocket>();

    serverSocket->Initialize();
    serverSocket->ResolveAddress();
    serverSocket->Connect();

    return serverSocket;
}
