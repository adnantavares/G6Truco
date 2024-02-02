#include "pch.h"
#include "Round.h"

Round::Round()
{

}

Round::Round(std::array<Player*, 4>& currentPlayers) {
    players = currentPlayers;
    for (auto& player : currentPlayers) {
        player->SetRaiseBetCallback(std::bind(&Round::OnRaiseBet, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void Round::OnRaiseBet(Player* player, int bet) {
    
}

void Round::StartRound() {
    roundCards.clear();
    deck.InitializeDeck();
    deck.Shuffle();

    DealCardsToPlayers();
    SetViraCard(TakeCardFromTopDeck(1)[0]);
    activePlayerIndex = 0; //TODO: Create a rule to define the activePlayer
    SetIsRoundOver(false);
}

void Round::NextPlayer()
{
    activePlayerIndex = (activePlayerIndex + 1) % players.size();
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

int Round::DetermineWinnerTeam() {
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

    int playerIndex = std::distance(players.begin(), std::find(players.begin(), players.end(), winningPlayer));
    return playerIndex % 2; // 0 is first team, 1 is second team
}

void Round::PlayCard(Card playedCard)
{
    bool isCardPlayed = players[activePlayerIndex]->PlayCard(playedCard);
    roundCards.push_back(std::make_pair(players[activePlayerIndex], playedCard));
    SetIsRoundOver(roundCards.size() == players.size());
}


#pragma region Getters and setters
int Round::GetActivePlayerIndex() const
{
    return activePlayerIndex;
}

Player* Round::GetActivePlayer()
{
    return players[GetActivePlayerIndex()];
}
Card Round::GetViraCard()
{
    return vira;
}
void Round::SetViraCard(Card viraCard)
{
    vira = viraCard;
}
std::array<Player*, 4> Round::GetAllPlayers()
{
    return players;
}
void Round::SetPlayers(std::array<Player*, 4> allPlayers)
{
    players = allPlayers;
}
bool Round::IsRoundOver() const
{
    return isRoundOver;
}
void Round::SetIsRoundOver(bool roundOver)
{
    isRoundOver = roundOver;
    if (isRoundOver) {
        RaiseRoundOverEvent();
    }
}
int Round::GetCurrentBet()
{
    return currentBet;
}
void Round::SetCurrentBet(int bet)
{
    currentBet = bet;
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
#pragma endregion


