// Game.h
#ifndef GAME_H
#define GAME_H

#include <array>
#include "Player.h"
#include "Deck.h"
#include "Round.h"

class Game {
private:
    //std::array<Player, 4> players; // The four players in the game
    //std::array<int, 2> scores; // Scores for the teams
    //Round round;
    //bool isGameOver; // Flag to check if the game has ended

    //void DealCards(); // Method to deal cards to players
    //void UpdateScores(int winningTeam, int points); // Method to update the scores
    //void PrepareRound(); //Shuffle deck and distribute cards to each player. Also, flips the 'vira' card.
    //void PlayRound(); // Plays a round
    //bool CheckGameOver(); // Checks if the game has ended

public:
    Game(); // Constructor
    void Start(); // Starts the game
    void Finish(); // Finishes the game and announces the winner

};

#endif // GAME_H