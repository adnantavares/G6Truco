#include "pch.h"
#include "ClientSocket.h"

using namespace G6Truco::Services::Connection;


shared_ptr<ClientSocket> ClientSocket::instance = nullptr;

ClientSocket::ClientSocket() { }

shared_ptr<ClientSocket> G6Truco::Services::Connection::ClientSocket::getInstance()
{
	if (instance == nullptr)
	{
		instance = std::shared_ptr<ClientSocket>(new ClientSocket());
		instance->Initialize("127.0.0.1", AI_PASSIVE);
	}

	return instance;
}
