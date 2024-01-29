#include "pch.h"
#include "SocketBase.h"

using namespace G6Truco::Services::Connection;
using namespace G6Truco::Services::Connection::Exceptions;
using namespace std;


SocketBase::SocketBase()
{
	addressInfo = NULL;
}

int SocketBase::ResolveAddress(const char* ipAddress)
{
	// Resolve the server address and port
	int retval = getaddrinfo(ipAddress, DEFAULT_PORT, &hints, &addressInfo);

	if (retval != 0)
	{
		cout << "getaddrinfo failed with error: " << retval << endl;
		WSACleanup();
		return retval;
	}

	return retval;
}

int SocketBase::Initialize(int addressInfoFlag)
{
	// Initialize Winsock
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		cout << "WSAStartup failed with error: " << result << endl;
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
		cout << "send failed with error: " << WSAGetLastError() << endl;
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
