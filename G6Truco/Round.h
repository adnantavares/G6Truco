#ifndef ROUND_H
#define ROUND_H
#include "Player.h"
#include "Deck.h"
#include "CardStrengthCalculator.h"
#include <array>
#include <vector>
#include <utility>

class Round {
private:
    Deck deck; // The deck of cards for the game
    std::array<std::unique_ptr<Player>, 4> players; // Pointers to the players in the round.
    std::vector<std::pair<Player*, Card>> roundCards; // Cards played in the round.
    Card vira;
    int currentBet; // The current bet (1, 3, 6, 9, 12).
    int activePlayerIndex;
    bool isRoundOver;
    std::function<void()> roundOverEvent;
    std::vector<int> possibleBets;

public:
    Round();
    void PlayCard(Card playedCard); // Method for players to play their cards.
    int DetermineWinnerTeam(); // Determines the winner of the round.
    void RaiseBet(); // Raises the current bet (Truco, Seis, Nove, Doze).
    void DealCardsToPlayers();
    std::vector<Card> TakeCardFromTopDeck(int numberOfCards);
    void OnRaiseBet(Player* player, int bet);
    void StartRound();
    void NextPlayer();
#pragma region Getters and Setters
    int GetActivePlayerIndex() const;
    Player* GetActivePlayer();
    Card GetViraCard();
    void SetViraCard(Card viraCard);
    const std::array<std::unique_ptr<Player>, 4>& Round::GetAllPlayers() const;
    void SetPlayers(std::array<std::unique_ptr<Player>, 4>&& allPlayers);
    bool IsRoundOver() const;
    void SetIsRoundOver(bool roundOver);
    int GetCurrentBet();
    void SetCurrentBet(int bet);
#pragma endregion

#pragma region Events
    void RoundOverEventListener(std::function<void()> callback);
    void RaiseRoundOverEvent();
#pragma endregion
};

#endif