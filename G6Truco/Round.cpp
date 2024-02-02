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
    isRoundOver = false;
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

void Round::PlayCard(Card playedCard)
{
    bool isCardPlayed = players[activePlayerIndex]->PlayCard(playedCard);
    roundCards.push_back(std::make_pair(players[activePlayerIndex], playedCard));
    isRoundOver = roundCards.size() == players.size();
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
#pragma endregion

