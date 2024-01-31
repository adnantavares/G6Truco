#include "pch.h"
#include "TrucoController.h"

TrucoController::TrucoController()
{
	player1.SetPlayerName(L"Pedro");
	player2.SetPlayerName(L"Priscila");
	player3.SetPlayerName(L"Adnan");
	player4.SetPlayerName(L"Danilo");
}

void TrucoController::OnCardPlayed(int playerIndex, Card card)
{

}

void TrucoController::OnBetCalled(int playerIndex, int bet)
{
	std::array<Player*, 4> players = { &player1, &player2, &player3, &player4 };
	Round round(players);

	player1.RaiseBetEvent(3);
}

void TrucoController::StartGame() {
	std::array<Player*, 4> players = { &player1, &player2, &player3, &player4 };
	Round round(players);

	round.StartRound();
	RaiseActivePlayerChangedEvent(round.GetActivePlayer());
	RaiseRoundInformationsChangedEvent(&round);

}

#pragma region Set events
void TrucoController::ActivePlayerChangedEvent(std::function<void(Player*)> callback) {
	activePlayerChangedEvent = callback;
}

void TrucoController::RoundInformationsChangedEvent(std::function<void(Round*)> callback)
{
	roundInformationsChangedEvent = callback;
}
#pragma endregion

#pragma region Trigger events
void TrucoController::RaiseActivePlayerChangedEvent(Player* currentPlayer)
{
	if (activePlayerChangedEvent) {
		activePlayerChangedEvent(currentPlayer);
	}
}

void TrucoController::RaiseRoundInformationsChangedEvent(Round* currentRoundInformations)
{
	if (roundInformationsChangedEvent) {
		roundInformationsChangedEvent(currentRoundInformations);
	}
}
#pragma endregion

