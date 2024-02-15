#include "pch.h"
#include "TrucoController.h"

TrucoController::TrucoController()
{
	firstPlayer = 0;
	std::array<std::unique_ptr<Player>, 4> players;

	round = std::make_unique<Round>();

	// Criação de jogadores humanos e CPUs.
	players[0] = HumanPlayer::Create(L"HUMAN-Pedro");
	players[1] = CPUPlayer::Create(L"BOT-Priscila", round.get());
	players[2] = HumanPlayer::Create(L"HUMAN-Adnan");
	players[3] = CPUPlayer::Create(L"BOT-Danilo", round.get());

	round->SetPlayers(std::move(players));
	round->RoundOverEventListener(std::bind(&TrucoController::NotifyRoundOver, this));
	
	StartRoundHandlerThread();
}

void TrucoController::PlayCard()
{
	round->NotifyPlayingAction();
}

void TrucoController::RaiseBet()
{
	round->RaiseBet();
}

void TrucoController::DenyBet()
{
	round->DenyBet();
}

void TrucoController::StartGame() {
	gamePoints.fill(0);
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

std::array<int, 2> TrucoController::GetGamePoints() {
	return gamePoints;
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
void TrucoController::StartRoundHandlerThread()
{
	// Creates handler thread to update UI and start another round
	roundHandlerThread = std::thread([this]() {
		std::unique_lock l(roundMutex);

		while (true)
		{
			roundConditionVariable.wait(l);

			int winnerTeamIndex = round->GetWinnerTeam();
			int currentBet = round->GetCurrentBet();
			gamePoints[winnerTeamIndex] += currentBet;
			firstPlayer = (firstPlayer + 1) % 4;
			round->StartRound(firstPlayer);
		}
	});

	roundHandlerThread.detach();
}
void TrucoController::RaiseActivePlayerChangedEvent(Player* currentPlayer)
{
	if (activePlayerChangedEvent) {
		activePlayerChangedEvent(currentPlayer);
	}
}
#pragma endregion

#pragma region Event listeners
void TrucoController::NotifyRoundOver()
{
	roundConditionVariable.notify_one();
}
#pragma endregion


