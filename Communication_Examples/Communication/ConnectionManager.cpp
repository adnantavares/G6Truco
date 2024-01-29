#include "ConnectionManager.h"

using namespace std;

void ConnectionManager::CreateRoom(int numberOfPlayers)
{
	if (numberOfPlayers % 2 != 0 && (numberOfPlayers < 2 || numberOfPlayers > 6))
	{
		cout << "Number of players not allowed!!!" << endl;
		return;
	}

	ServerSocket::StartListening(numberOfPlayers - 1);

}

void ConnectionManager::ConnectToRoom(string ipAdress, string port)
{
}
