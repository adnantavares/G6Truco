#include "SocketBase.h"
#include "BufferOverflowException.h"


SocketBase::SocketBase()
{
	addressInfo = NULL;
}

int SocketBase::ResolveAddress(const char* ipAddress = NULL)
{
	// Resolve the server address and port
	int retval = getaddrinfo(ipAddress, DEFAULT_PORT, &hints, &addressInfo);

	if (retval != 0)
	{
		std::cout << "getaddrinfo failed with error: " << retval << std::endl;
		WSACleanup();
		return retval;
	}

	return retval;
}

int SocketBase::Initialize(int addressInfoFlag = NULL)
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
	
	if (addressInfoFlag != NULL)
	{
		hints.ai_flags = addressInfoFlag;
	}
}

int SocketBase::SendData(SOCKET remoteSocket, const char* data)
{
	int retValue = -1; // Error
	int dataLength = (int)strlen(data);

	if (dataLength > DEFAULT_BUFFER_LENGTH)
	{
		throw BufferOverflowException("Buffer overflow!", dataLength, DEFAULT_BUFFER_LENGTH);
	}

	retValue = send(remoteSocket, data, dataLength, 0);

	if (retValue == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		closesocket(remoteSocket);
		WSACleanup();
		return retValue;
	}

	return retValue;
}

int SocketBase::ReceiveData(SOCKET remoteSocket, char* data)
{
	int bytesReceived = 0;

	bytesReceived = recv(remoteSocket, data, DEFAULT_BUFFER_LENGTH, 0);

	if (bytesReceived > DEFAULT_BUFFER_LENGTH)
	{
		throw BufferOverflowException("Buffer overflow!", bytesReceived, DEFAULT_BUFFER_LENGTH);
	}
	else if (bytesReceived > 0)
	{
		std::cout << "Bytes received: " << bytesReceived << std::endl;
	}
	else if (bytesReceived == 0)
	{
		std::cout << "Connection closed\n" << std::endl;
	}
	else
	{
		std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
	}

	return bytesReceived;
}
