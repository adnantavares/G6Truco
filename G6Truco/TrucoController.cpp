#include "pch.h"
#include "TrucoController.h"

TrucoController::TrucoController()
{
	player1.SetPlayerName(L"Pedro");
	player2.SetPlayerName(L"Priscila");
	player3.SetPlayerName(L"Adnan");
	player4.SetPlayerName(L"Danilo");
	TrucoController::round.SetPlayers({ &player1, &player2, &player3, &player4 });
	TrucoController::round.RoundOverEventListener(std::bind(&TrucoController::HandleRoundOver, this));
}

void TrucoController::PlayCard(int cardIndex)
{
	if (cardIndex < TrucoController::round.GetActivePlayer()->GetHand().size())
	{
		TrucoController::round.PlayCard(TrucoController::round.GetActivePlayer()->GetHand()[cardIndex]);
		TrucoController::round.NextPlayer();
	}
}

void TrucoController::RaiseBet()
{
	TrucoController::round.RaiseBet();
}

void TrucoController::StartGame() {
	TrucoController::round.SetPlayers({ &player1, &player2, &player3, &player4 });

	TrucoController::round.StartRound();
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

#pragma region Event listeners
void TrucoController::HandleRoundOver()
{
	int winnerTeamIndex = TrucoController::round.DetermineWinnerTeam();
	int currentBet = TrucoController::round.GetCurrentBet();
	//TODO: Check if has a game winner team, before start a new round
	TrucoController::round.StartRound();
}
#pragma endregion


