#include "pch.h"
#include "Round.h"


std::mutex Round::playingMutex;
std::condition_variable Round::playingConditionVariable;


Round::Round()
{
	activePlayerIndex = 0;
	possibleBets = { 1, 3, 6, 9, 12 };

	StartPlayingThread();
}

void Round::OnRaiseBet(Player* player, int betDecision) {
	if (currentBet == betDecision) {
		// CPU only accepted truco
		// Truco button must be disabled
		currentTrucoCall = TrucoCallType::CPU_ACCEPTED;
	}
	else if (currentBet < betDecision) {
		// CPU raised bet
		// Truco button must be updated to the next value
		currentTrucoCall = TrucoCallType::CPU_RAISED;
		NextBet();
	}
	else
	{
		DenyBet(player);
	}
	
	if (!IsRoundOver()) {
		RaiseRoundInformationChangedEvent();
	}
}

void Round::StartRound(int firstPlayer) {
	currentBet = 1;
	currentTrucoCall = TrucoCallType::NONE;
	winningCard.reset();
	roundCards.clear();
	deck.InitializeDeck();
	deck.Shuffle();
	CPUPlayer::ResetPlayerBet();

	DealCardsToPlayers();
	SetViraCard(TakeCardFromTopDeck(1)[0]);

	activePlayerIndex = firstPlayer;
	roundWinnerTeam = -1;
	points = std::vector(2, 0);

	RaiseRoundInformationChangedEvent();

	if (!IsHumanPlayer()) {
		//If first player is a bot player, play its card automatically.
		CPUPlayer::NotifyPlayers();
	}
}

void Round::NextPlayer()
{
	if (roundCards.size() == players.size()) {
		DetermineRoundPoint(activePlayerIndex);
	}
	else {
		activePlayerIndex = (activePlayerIndex + 1) % players.size();
	}

	if (!IsRoundOver()) {
		if (!IsHumanPlayer()) {
			//If next player is a bot player, play its card automatically.
			CPUPlayer::NotifyPlayers();
		}

		RaiseRoundInformationChangedEvent();
	}
}

bool Round::NewCardIsStronger(const Card& newCard, const Card& currentWinningCard)
{
	CardStrengthCalculator strengthCalculator(vira);
	double newCardStrength = strengthCalculator.GetCardStrength(newCard);
	double currentWinningCardStrength = strengthCalculator.GetCardStrength(currentWinningCard);

	return newCardStrength > currentWinningCardStrength;

}

void Round::DealCardsToPlayers() {
	for (auto& player : players) {
		std::vector<Card> hand = TakeCardFromTopDeck(3);
		player->SetHand(hand);
	}
}

std::vector<Card> Round::TakeCardFromTopDeck(int numberOfCards)
{
	std::vector<Card> hand;
	for (int i = 0; i < numberOfCards; ++i) {
		hand.push_back(deck.TakeTopCard());
	}
	return hand;
}

void Round::RemovePlayedCards()
{
	roundCards.clear();

	for (auto& player : players) {
		player->RemoveSelectedCard();
	}
}

void Round::StartPlayingThread()
{
	// Creates playing thread to handle the player actions
	playingThread = std::thread([this]() {
		std::unique_lock l(playingMutex);

		while (true)
		{
			playingConditionVariable.wait(l);

			this->PlayCard();
			this->NextPlayer();
		}
	});

	playingThread.detach();
}

void Round::DetermineRoundPoint(int& playerIndex)
{
	double highestStrength = 0;
	Player* winningPlayer = nullptr;
	CardStrengthCalculator strengthCalculator(vira);

	for (const auto& roundCard : roundCards) {
		double strength = strengthCalculator.GetCardStrength(roundCard.second);
		if (strength > highestStrength) {
			highestStrength = strength;
			winningPlayer = roundCard.first;
		}
	}

	auto it = std::find_if(players.begin(), players.end(), [&winningPlayer](const std::unique_ptr<Player>& p) { return p.get() == winningPlayer; });
	if (it != players.end()) {
		playerIndex = std::distance(players.begin(), it);
		points[playerIndex % 2] += 1; // 0 is first team, 1 is second team
	}

	// After computing the point, the cards of the table are removed 
	RemovePlayedCards();
}

void Round::NotifyPlayingAction()
{
	playingConditionVariable.notify_one();
}

void Round::RaiseBet()
{
	
	NextBet();
	// Notify one of the CPU players, who will decide to raise the bet, accept current one or leave the round
	CPUPlayer::NotifyPlayers(false);
}

void Round::DenyBet(Player* player)
{
	PreviousBet();

	if (player == nullptr || IsHumanPlayer(player)) {
		points[0] = 0;  // Human Team
		points[1] = 2;  // CPU Team

		IsRoundOver();
	}
	else {
		// In case CPU denies truco, the points are configured to finish the round with human team victory.
		points[0] = 2;  // Human Team
		points[1] = 0;  // CPU Team
	}
}

void Round::PlayCard()
{
	Card playedCard = players[activePlayerIndex]->PlayCard();
	DefineWinningCard(playedCard);
	roundCards.push_back(std::make_pair(players[activePlayerIndex].get(), playedCard));
}

void Round::PreviousBet()
{
	auto it = std::find(possibleBets.begin(), possibleBets.end(), currentBet);
	if (it != possibleBets.end() && std::next(it) != possibleBets.end()) {
		// Next bet, if it is not the last one.
		currentBet = *(std::prev(it));
	}
}

void Round::NextBet()
{
	auto it = std::find(possibleBets.begin(), possibleBets.end(), currentBet);
	if (it != possibleBets.end() && std::next(it) != possibleBets.end()) {
		// Next bet, if it is not the last one.
		currentBet = *(std::next(it));
	}
}

#pragma region Getters and setters
int Round::GetWinnerTeam() const {
	return roundWinnerTeam;
}

int Round::GetActivePlayerIndex() const
{
	return activePlayerIndex;
}

Player* Round::GetActivePlayer()
{
	return players[GetActivePlayerIndex()].get();
}

Card Round::GetViraCard()
{
	return vira;
}

void Round::SetViraCard(Card viraCard)
{
	vira = viraCard;

	for (auto& player : players) {
		if (!IsHumanPlayer(player.get())) {
			CPUPlayer* cpuPlayer = dynamic_cast<CPUPlayer*>(player.get());
			cpuPlayer->SetCurrentViraCard(vira);
		}
	}
}

const std::optional<std::pair<Player*, Card>> Round::GetWinningCard()
{
	return winningCard;
}

const std::array<std::unique_ptr<Player>, 4>& Round::GetAllPlayers() const
{
	return players;
}

void Round::SetPlayers(std::array<std::unique_ptr<Player>, 4>&& allPlayers)
{
	players = std::move(allPlayers);

	for (auto& player : players) {
		if (!IsHumanPlayer(player.get())) {
			CPUPlayer* cpuPlayer = dynamic_cast<CPUPlayer*>(player.get());
			cpuPlayer->SetRaiseBetCallback(std::bind(&Round::OnRaiseBet, this, std::placeholders::_1, std::placeholders::_2));
		}
	}
}

bool Round::IsRoundOver()
{
	auto it = std::max_element(points.begin(), points.end()); // iterator to the team with more points
	int teamIndex = std::distance(points.begin(), it); // 0 is human team, 1 is CPU team
	int teamPoints = *it;

	if (teamPoints < 2) {
		isRoundOver = false;
	}
	else {
		roundWinnerTeam = teamIndex;
		isRoundOver = true;	
		RaiseRoundOverEvent();
	}

	return isRoundOver;
}

int Round::GetCurrentBet()
{
	return currentBet;
}

void Round::SetCurrentBet(int bet)
{
	currentBet = bet;
}

int Round::GetCurrentTrucoCall()
{
	return currentTrucoCall;
}

bool Round::IsHumanPlayer()
{
	Player* activePlayer = GetActivePlayer();
	return IsHumanPlayer(activePlayer);
}

bool Round::IsHumanPlayer(Player* player)
{
	HumanPlayer* humanPlayer = dynamic_cast<HumanPlayer*>(player);
	return humanPlayer != nullptr;
}

void Round::DefineWinningCard(Card playedCard)
{
	if (!winningCard || NewCardIsStronger(playedCard, winningCard->second)) {
		winningCard = std::make_optional(std::make_pair(players[activePlayerIndex].get(), playedCard));
	}
}

std::vector<int> Round::GetPoints()
{
	return points;
}

CPUPlayer* Round::GetCPUActivePlayer()
{
	Player* activePlayer = GetActivePlayer();
	CPUPlayer* cpuPlayer = dynamic_cast<CPUPlayer*>(activePlayer);
	return cpuPlayer;
}
#pragma endregion

#pragma region Events
void Round::RoundOverEventListener(std::function<void()> callback)
{
	roundOverEvent = callback;
}

void Round::RaiseRoundOverEvent()
{
	if (roundOverEvent) {
		roundOverEvent();
	}
}

void Round::RoundInformationChangedListener(std::function<void(Round*)> callback)
{
	roundInformationChangedEvent = callback;
}

void Round::RaiseRoundInformationChangedEvent()
{
	if (roundInformationChangedEvent) {
		roundInformationChangedEvent(this);
	}
}
#pragma endregion


