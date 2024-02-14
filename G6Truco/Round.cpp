#include "pch.h"
#include "Round.h"


Round::Round()
{
    activePlayerIndex = 0;
    possibleBets = { 1, 3, 6, 9, 12 };
}

void Round::OnRaiseBet(Player* player, int bet) {
    
}

void Round::StartRound(int firstPlayer) {
    currentBet = 1;
    winningCard.reset();
    roundCards.clear();
    deck.InitializeDeck();
    deck.Shuffle();

    DealCardsToPlayers();
    SetViraCard(TakeCardFromTopDeck(1)[0]);

    activePlayerIndex = firstPlayer;
    roundWinnerTeam = -1;
    points = std::vector(2, 0);

    //If first player is a bot player, play its card automatically.
    CPUPlayer::NotifyPlayers();
}

void Round::NextPlayer()
{
    if (roundCards.size() == players.size()) {
        DetermineRoundPoint(activePlayerIndex);
    }
    else {
        activePlayerIndex = (activePlayerIndex + 1) % players.size();
    }
    
    if (!IsRoundOver() && !IsHumanPlayer())
    {
        //If next player is a bot player, play its card automatically.
        CPUPlayer::NotifyPlayers();
    }

    RaiseRoundInformationChangedEvent();
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

    // After computing the point, the cards of the table are removed to 
    roundCards.clear();
}

int Round::GetWinnerTeam() const {
    return roundWinnerTeam;
}

void Round::RaiseBet()
{
    auto it = std::find(possibleBets.begin(), possibleBets.end(), currentBet);
    if (it != possibleBets.end() && std::next(it) != possibleBets.end()) {
        // Next bet, if it is not the last one.
        currentBet = *(std::next(it));
    }
}

void Round::PlayCard()
{
    Card playedCard = players[activePlayerIndex]->PlayCard();
    DefineWinningCard(playedCard);
    roundCards.push_back(std::make_pair(players[activePlayerIndex].get(), playedCard));
}


#pragma region Getters and setters
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
}

bool Round::IsRoundOver() 
{
    auto it = std::max_element(points.begin(), points.end()); // iterator to the team with more points
    int teamIndex = std::distance(points.begin(), it); // 0 is first team, 1 is second team
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


