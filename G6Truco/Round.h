#ifndef ROUND_H
#define ROUND_H
#include "Player.h"
#include "HumanPlayer.h"
#include "Deck.h"
#include "CardStrengthCalculator.h"
#include <array>
#include <vector>
#include <utility>
#include <optional>
#include "CPUPlayer.h"

// Forward Declaration
class CPUPlayer;


class Round {
private:
    int roundWinnerTeam;
    std::vector<int> points;
    Deck deck; // The deck of cards for the game
    std::array<std::unique_ptr<Player>, 4> players; // Pointers to the players in the round.
    std::vector<std::pair<Player*, Card>> roundCards; // Cards played in the round.
    Card vira;
    std::optional<std::pair<Player*, Card>> winningCard; //The stronghest card in current round
    int currentBet; // The current bet (1, 3, 6, 9, 12).
    int activePlayerIndex;
    bool isRoundOver;
    std::function<void()> roundOverEvent;
    std::vector<int> possibleBets;

    void DetermineRoundPoint(int& playerIndex); // Determines the winner of the round point.

public:
    Round();
    void PlayCard(); // Method for players(Humans or CPU) to play their cards.
    void RaiseBet(); // Raises the current bet (Truco, Seis, Nove, Doze).
    void DealCardsToPlayers();
    std::vector<Card> TakeCardFromTopDeck(int numberOfCards);
    void OnRaiseBet(Player* player, int bet);
    void StartRound(int firstPlayer);
    void NextPlayer();
    bool Round::NewCardIsStronger(const Card& newCard, const Card& currentWinningCard);
#pragma region Getters and Setters
    int GetActivePlayerIndex() const;
    Player* GetActivePlayer();
    Card GetViraCard();
    void SetViraCard(Card viraCard);
    const std::optional<std::pair<Player*, Card>> GetWinningCard();
    const std::array<std::unique_ptr<Player>, 4>& Round::GetAllPlayers() const;
    void SetPlayers(std::array<std::unique_ptr<Player>, 4>&& allPlayers);
    bool IsRoundOver();
    int GetCurrentBet();
    int GetWinnerTeam() const; // Determines the winner of the round.
    void SetCurrentBet(int bet);
    bool IsHumanPlayer();
    bool IsHumanPlayer(Player* player);
    void DefineWinningCard(Card playedCard);
    CPUPlayer* GetCPUActivePlayer(); //nullPtr if the active player is not a CPUPlayer
#pragma endregion

#pragma region Events
    void RoundOverEventListener(std::function<void()> callback);
    void RaiseRoundOverEvent();
#pragma endregion
};

#endif