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
    deck.InitializeDeck();
    deck.Shuffle();

    DealCardsToPlayers();
    SetViraCard(TakeCardFromTopDeck(1)[0]);
    activePlayerIndex = 0; //TODO: Create a rule to define the activePlayer
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
#pragma endregion

