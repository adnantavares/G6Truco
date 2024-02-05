#include "pch.h"
#include "Round.h"

Round::Round()
{
    possibleBets = { 1, 3, 6, 9, 12 };
}

void Round::OnRaiseBet(Player* player, int bet) {
    
}

void Round::StartRound() {
    currentBet = 1;
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

    auto it = std::find_if(players.begin(), players.end(), [&winningPlayer](const std::unique_ptr<Player>& p) { return p.get() == winningPlayer; });
    if (it != players.end()) {
        int playerIndex = std::distance(players.begin(), it);
        return playerIndex % 2; // 0 is first team, 1 is second team
    }

    return -1;
}

void Round::RaiseBet()
{
    auto it = std::find(possibleBets.begin(), possibleBets.end(), currentBet);
    if (it != possibleBets.end() && std::next(it) != possibleBets.end()) {
        // Next bet, if it is not the last one.
        currentBet = *(std::next(it));
    }
}

void Round::PlayCard(Card playedCard)
{
    bool isCardPlayed = players[activePlayerIndex]->PlayCard(playedCard);
    roundCards.push_back(std::make_pair(players[activePlayerIndex].get(), playedCard));
    SetIsRoundOver(roundCards.size() == players.size());
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
}
const std::array<std::unique_ptr<Player>, 4>& Round::GetAllPlayers() const
{
    return players;
}
void Round::SetPlayers(std::array<std::unique_ptr<Player>, 4>&& allPlayers)
{
    players = std::move(allPlayers);
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


