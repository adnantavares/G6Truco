#include "pch.h"
#include "ConnectionManager.h"

using namespace G6Truco::Services::Connection;


void ConnectionManager::CreateRoom(int numberOfPlayers)
{
	if (numberOfPlayers % 2 != 0 && (numberOfPlayers < 2 || numberOfPlayers > 6))
	{
		cout << "Number of players not allowed!!!" << endl;
		return;
	}

	std::shared_ptr<ServerSocket> serverSocket = ServerSocket::getInstance();
}

void ConnectionManager::ConnectToRoom(string ipAdress, string port)
{
}