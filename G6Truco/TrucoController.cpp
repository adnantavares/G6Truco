#include "pch.h"
#include "TrucoController.h"

void TrucoController::OnCardPlayed(int playerIndex, Card card)
{

}

void TrucoController::OnBetCalled(int playerIndex, int bet)
{
	std::array<Player*, 4> players = { &player1, &player2, &player3, &player4 };
	Round round(players);

	player1.RaiseBetEvent(3);
}