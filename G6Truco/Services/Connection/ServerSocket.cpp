#include "pch.h"
#include "ServerSocket.h"

using namespace G6Truco::Services::Connection;


shared_ptr<ServerSocket> ServerSocket::instance = nullptr;

ServerSocket::ServerSocket() { }

shared_ptr<ServerSocket> ServerSocket::getInstance()
{
	if (instance == nullptr)
	{
		instance = std::shared_ptr<ServerSocket>(new ServerSocket());
		instance->Initialize(AI_PASSIVE);
	}

	return instance;
}

vector<SOCKET> ServerSocket::ConnectToClients(int numberOfConnections)
{


	return vector<SOCKET>();
}
