#include "pch.h"
#include "TrucoController.h"

TrucoController::TrucoController()
{
	firstPlayer = 0;
	std::array<std::unique_ptr<Player>, 4/*NUMBER_OF_PLAYERS*/> players;

	round = std::make_unique<Round>();

	// Criação de jogadores humanos e CPUs.
	players[0] = HumanPlayer::Create(L"HUMAN-Pedro");
	players[1] = CPUPlayer::Create(L"BOT-Priscila", round.get());
	players[2] = HumanPlayer::Create(L"HUMAN-Adnan");
	players[3] = CPUPlayer::Create(L"BOT-Danilo", round.get());

	round->SetPlayers(std::move(players));
	round->RoundOverEventListener(std::bind(&TrucoController::HandleRoundOver, this));
}

void TrucoController::PlayCard()
{
	round->PlayCard();
	round->NextPlayer();
}

void TrucoController::RaiseBet()
{
	round->RaiseBet();
}

void TrucoController::StartGame() {

	round->StartRound(firstPlayer);
	RaiseActivePlayerChangedEvent(round->GetActivePlayer());
}

//True if player is a HumanPlayer
bool TrucoController::IsActivePlayerHuman()
{
	return round->IsHumanPlayer();
}

bool TrucoController::TrySetSelectedCardIndex(int index)
{
	if (IsActivePlayerHuman()) {
		auto* humanPlayer = dynamic_cast<HumanPlayer*>(round->GetActivePlayer());
		if (humanPlayer) {
			humanPlayer->SetSelectCardIndex(index);
			return true;
		}
	}
	return false;
}

#pragma region Set events
void TrucoController::ActivePlayerChangedEventListener(std::function<void(Player*)> callback) {
	activePlayerChangedEvent = callback;
}

void TrucoController::RoundInformationChangedEventListener(std::function<void(Round*)> callback)
{
	round->RoundInformationChangedListener(callback);
}
#pragma endregion

#pragma region Trigger events
void TrucoController::RaiseActivePlayerChangedEvent(Player* currentPlayer)
{
	if (activePlayerChangedEvent) {
		activePlayerChangedEvent(currentPlayer);
	}
}
#pragma endregion

#pragma region Event listeners
void TrucoController::HandleRoundOver()
{
	int winnerTeamIndex = round->GetWinnerTeam();
	int currentBet = round->GetCurrentBet();
	
	//TODO: Check if has a game winner team, before start a new round
	firstPlayer = (firstPlayer + 1) % NUMBER_OF_PLAYERS;
	round->StartRound(firstPlayer);
}
#pragma endregion


