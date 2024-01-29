#pragma once

#include <iostream>
#include "ClientSocket.h"
#include "ServerSocket.h"


class ConnectionManager
{
private:



public:
	static void CreateRoom(int numberOfPlayers);
	static void ConnectToRoom(string ipAdress, string port);

	/*
	void Truco(Player player, int points);
	void Leave(Player player);
	void PlayCard(Player player, Card card, bool isHidden);
	void ShowCards(Player player, Player partner, vector<Card> cards);
	void SendCards(Player player, vector<Card> cards);
	*/
};

