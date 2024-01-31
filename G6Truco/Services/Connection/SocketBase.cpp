#include "pch.h"
#include "SocketBase.h"

using namespace G6Truco::Services::Connection;
using namespace G6Truco::Services::Connection::Exceptions;
using namespace std;


SocketBase::SocketBase()
{
	commSocket = NULL;
	addressInfo = NULL;
}

int SocketBase::InitSocket(const char* ipAddress)
{
	// Create a SOCKET for the server to listen for client connections.
	commSocket = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);

	if (commSocket == INVALID_SOCKET)
	{
		std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(addressInfo);
		WSACleanup();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int SocketBase::ResolveAddress(const char* ipAddress)
{
	// Resolve the server address and port
	int retval = getaddrinfo(ipAddress, DEFAULT_PORT, &hints, &addressInfo);

	if (retval != 0)
	{
		cout << "getaddrinfo failed with error: " << retval << endl;
		WSACleanup();
	}

	return retval;
}

int SocketBase::Bind()
{
	// Setup the TCP listening socket
	int result = bind(commSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);

	if (result == SOCKET_ERROR)
	{
		std::cout << "bind failed with error: " << WSAGetLastError() << std::endl;
		freeaddrinfo(addressInfo);
		closesocket(commSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}

	freeaddrinfo(addressInfo);
	return EXIT_SUCCESS;
}

int SocketBase::Listen()
{
	int retVal = listen(commSocket, SOMAXCONN);

	if (retVal == SOCKET_ERROR)
	{
		std::cout << "listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(commSocket);
		WSACleanup();
	}

	return retVal;
}

int SocketBase::Accept()
{
	// Accept a client socket
	SOCKET ClientSocket = accept(commSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
		closesocket(commSocket);
		WSACleanup();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int SocketBase::Initialize(const char* ipAddress, int addressInfoFlag)
{
	// Initialize Winsock
	int retVal = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (retVal != 0)
	{
		cout << "WSAStartup failed with error: " << retVal << endl;
		return retVal;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (addressInfoFlag != NULL)
	{
		hints.ai_flags = addressInfoFlag;
	}

	retVal = ResolveAddress(ipAddress);
	retVal = InitSocket(ipAddress);

	return retVal;
}

int SocketBase::Connect()
{
	// Connect to server.
	int retVal = connect(commSocket, addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
	
	if (retVal == SOCKET_ERROR) 
	{
		closesocket(commSocket);
		commSocket = INVALID_SOCKET;
	}

	freeaddrinfo(addressInfo);
	return retVal;
}

int SocketBase::SendData(const char* data)
{
	int retValue = -1; // Error
	int dataLength = (int)strlen(data);

	if (dataLength > DEFAULT_BUFFER_LENGTH)
	{
		throw BufferOverflowException("Buffer overflow!", dataLength, DEFAULT_BUFFER_LENGTH);
	}

	retValue = send(commSocket, data, dataLength, 0);

	if (retValue == SOCKET_ERROR)
	{
		cout << "send failed with error: " << WSAGetLastError() << endl;
		closesocket(commSocket);
		WSACleanup();
	}

	return retValue;
}

int SocketBase::ReceiveData(char* data)
{
	int bytesReceived = 0;

	bytesReceived = recv(commSocket, data, DEFAULT_BUFFER_LENGTH, 0);

	if (bytesReceived > DEFAULT_BUFFER_LENGTH)
	{
		throw BufferOverflowException("Buffer overflow!", bytesReceived, DEFAULT_BUFFER_LENGTH);
	}
	else if (bytesReceived > 0)
	{
		cout << "Bytes received: " << bytesReceived << endl;
	}
	else if (bytesReceived == 0)
	{
		cout << "Connection closed\n" << endl;
	}
	else
	{
		cout << "recv failed with error: " << WSAGetLastError() << endl;
	}

	return bytesReceived;
}
