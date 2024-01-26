#ifndef ROUND_H
#define ROUND_H
#include "Player.h"

class Round {
private:
    std::array<Player*, 4> players; // Pointers to the players in the round.
    std::vector<Card> playedCards; // Cards played in the round.
    int currentBet; // The current bet (1, 3, 6, 9, 12).

public:
    Round(std::array<Player*, 4>& players); // Constructor receiving the players.
    void playCards(); // Method for players to play their cards.
    int determineWinner(); // Determines the winner of the round.
    void raiseBet(int raise); // Raises the current bet (Truco, Seis, Nove, Doze).
};

#endif