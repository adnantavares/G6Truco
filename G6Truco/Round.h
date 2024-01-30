#ifndef ROUND_H
#define ROUND_H
#include "Player.h"
#include "Deck.h"
#include <array>
#include <vector>

class Round {
private:
    Deck deck; // The deck of cards for the game
    std::array<Player*, 4> players; // Pointers to the players in the round.
    std::vector<Card> playedCards; // Cards played in the round.
    Card vira;
    int currentBet; // The current bet (1, 3, 6, 9, 12).
    int activePlayerIndex;

public:
    Round();
    Round(std::array<Player*, 4>& players); // Constructor receiving the players.
    void PlayCards(); // Method for players to play their cards.
    int DetermineWinner(); // Determines the winner of the round.
    void RaiseBet(int raise); // Raises the current bet (Truco, Seis, Nove, Doze).
    void DealCardsToPlayers();
    void OnRaiseBet(Player* player, int bet);
    void StartRound();
    void NextPlayer();
    int GetActivePlayerIndex() const;
};

#endif