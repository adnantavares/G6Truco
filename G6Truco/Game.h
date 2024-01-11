// Game.h
#ifndef GAME_H
#define GAME_H

#include <array>
#include "Player.h"
#include "Deck.h"
#include "Round.h"

class Game {
private:
    Deck deck; // The deck of cards for the game
    std::array<Player, 4> players; // The four players in the game
    std::array<int, 2> scores; // Scores for the teams
    bool isGameOver; // Flag to check if the game has ended

    void dealCards(); // Method to deal cards to players
    void updateScores(int winningTeam, int points); // Method to update the scores
    bool checkGameOver(); // Checks if the game has ended

public:
    Game(); // Constructor
    void start(); // Starts the game
    void playRound(); // Plays a round
    void finish(); // Finishes the game and announces the winner

};

#endif // GAME_H