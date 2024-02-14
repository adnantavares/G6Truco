#include "pch.h"
#include "CPUPlayer.h"


std::mutex CPUPlayer::playerMutex;
std::condition_variable CPUPlayer::playerConditionVariable;


CPUPlayer::CPUPlayer(const CString& name) : Player(PlayerType::Cpu)
{
	SetPlayerName(name);
}

std::unique_ptr<CPUPlayer> CPUPlayer::Create(const CString& name, Round& round, std::function<void(Round*)>* callback)
{
	std::unique_ptr<CPUPlayer> player = std::unique_ptr<CPUPlayer>(new CPUPlayer(name));
	player->MonitorRoundState(round, callback);

	return std::move(player);
}

void CPUPlayer::NotifyPlayers()
{
	playerConditionVariable.notify_all();
}

Card CPUPlayer::PlayCard()
{
	// If no card is winning yet, play the weakest card
	if (!winningCard.has_value()) {
		return PlaySelectedCard(FindWeakestCard());
	}

	const auto& [winningPlayer, winningCardValue] = winningCard.value();

	// If the winning card is from the opposing team, try to play a stronger card
	if (winningPlayer->playerType == Player::PlayerType::Human) {
		auto strongerCard = FindStrongerCard(winningCardValue);
		if (strongerCard) {
			return PlaySelectedCard(*strongerCard);
		}
		else {
			// If any card is stronger than 'winningCard', play the weakest card
			return PlaySelectedCard(FindWeakestCard());
		}
	}
	else {
		// Play the weakest card, since the 'winningCard' is from another CPUPlayer
		return PlaySelectedCard(FindWeakestCard());
	}
}

Card CPUPlayer::PlaySelectedCard(const Card& cardToPlay) {
	auto it = std::find(hand.begin(), hand.end(), cardToPlay);
	if (it != hand.end()) {
		hand.erase(it);
	}
	return cardToPlay;
}

void CPUPlayer::SetWinningCard(std::optional<std::pair<Player*, Card>> newWinningCard)
{
	winningCard = newWinningCard;
}

void CPUPlayer::SetCurrentViraCard(Card viraCard)
{
	currentViraCard = viraCard;
}

Card CPUPlayer::FindWeakestCard() const
{
	auto it = std::min_element(hand.begin(), hand.end(), [this](const Card& a, const Card& b) {
		CardStrengthCalculator strengthCalculator(currentViraCard);
		return strengthCalculator.GetCardStrength(a) < strengthCalculator.GetCardStrength(b);
		});
	return *it;
}

std::optional<Card> CPUPlayer::FindStrongerCard(const Card& targetCard) const
{
	CardStrengthCalculator strengthCalculator(currentViraCard);
	auto it = std::find_if(hand.begin(), hand.end(), [&](const Card& card) {
		return strengthCalculator.GetCardStrength(card) > strengthCalculator.GetCardStrength(targetCard);
		});

	if (it != hand.end()) {
		return *it;
	}
	else {
		return std::nullopt; // It don't have any stronger card
	}
}

void CPUPlayer::MonitorRoundState(Round& round, std::function<void(Round*)>* callback)
{
	// Bot player keeps monitoring the round in order to play when it is its turn
	std::thread activePlayerThread([this, &round, callback]() {
		std::unique_lock l(playerMutex);

		while (true)
		{
			CPUPlayer* cpuPlayer = round.GetCPUActivePlayer();

			//If next player is this bot player, play its card automatically.
			if (cpuPlayer != nullptr && cpuPlayer->GetPlayerName() == this->GetPlayerName())
			{
				this->SetWinningCard(round.GetWinningCard()); //Set winningCard in order to CPU player knows witch card to play
				
				// Waits Human Player card being invalidated, before playing CPU Player card
				std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(PLAY_INTERVAL));
				
				round.PlayCard();
				round.NextPlayer();
				if (callback != nullptr) {
					(*callback)(&round);
				}
			}
			
			playerConditionVariable.wait(l);
		}
	});

	activePlayerThread.detach();
}

