#include "pch.h"
#include "TrucoController.h"

TrucoController::TrucoController()
{
	firstPlayer = 0;
	std::array<std::unique_ptr<Player>, 4/*NUMBER_OF_PLAYERS*/> players;

	// Criação de jogadores humanos e CPUs.
	players[0] = HumanPlayer::Create(L"HUMAN-Pedro");
	players[1] = CPUPlayer::Create(L"BOT-Priscila", TrucoController::round);
	players[2] = HumanPlayer::Create(L"HUMAN-Adnan");
	players[3] = CPUPlayer::Create(L"BOT-Danilo", TrucoController::round);

	TrucoController::round.SetPlayers(std::move(players));
	TrucoController::round.RoundOverEventListener(std::bind(&TrucoController::HandleRoundOver, this));
}

void TrucoController::PlayCard()
{
	TrucoController::round.PlayCard();
	TrucoController::round.NextPlayer();
	RaiseRoundInformationsChangedEvent(&round);
}

void TrucoController::RaiseBet()
{
	TrucoController::round.RaiseBet();
}

void TrucoController::StartGame() {

	TrucoController::round.StartRound(firstPlayer);
	RaiseActivePlayerChangedEvent(round.GetActivePlayer());
	RaiseRoundInformationsChangedEvent(&round);
}

//True if player is a HumanPlayer
bool TrucoController::IsActivePlayerHuman()
{
	return round.IsHumanPlayer();
}

bool TrucoController::TrySetSelectedCardIndex(int index)
{
	if (IsActivePlayerHuman()) {
		auto* humanPlayer = dynamic_cast<HumanPlayer*>(round.GetActivePlayer());
		if (humanPlayer) {
			humanPlayer->SetSelectCardIndex(index);
			return true;
		}
	}
	return false;
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
	firstPlayer = (firstPlayer + 1) % NUMBER_OF_PLAYERS;
	TrucoController::round.StartRound(firstPlayer);
}
#pragma endregion


