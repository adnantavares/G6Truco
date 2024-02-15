#ifndef ROUND_H
#define ROUND_H
#include "Player.h"
#include "HumanPlayer.h"
#include "Deck.h"
#include "CardStrengthCalculator.h"
#include <array>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>
#include "CPUPlayer.h"

// Forward Declaration
class CPUPlayer;


class Round {
private:
    static std::mutex playingMutex;
    static std::condition_variable playingConditionVariable;
    std::thread playingThread;

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
    std::function<void(Round*)> roundInformationChangedEvent;
    std::function<void()> roundOverEvent;
    std::vector<int> possibleBets;

    void NextBet();
    void RemovePlayedCards();
    void StartPlayingThread();
    void DetermineRoundPoint(int& playerIndex); // Determines the winner of the round point.
    void PlayCard(); // Method for players(Humans or CPU) to play their cards.
    void NextPlayer();

public:
    Round();

    void NotifyPlayingAction();
    void RaiseBet(); // Raises the current bet (Truco, Seis, Nove, Doze).
    void DenyBet(); // Deny bet from the main thread.
    void DealCardsToPlayers();
    std::vector<Card> TakeCardFromTopDeck(int numberOfCards);
    void OnRaiseBet(Player* player, int betDecision);
    void StartRound(int firstPlayer);
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
    std::vector<int> GetPoints();
    CPUPlayer* GetCPUActivePlayer(); //nullPtr if the active player is not a CPUPlayer
#pragma endregion

#pragma region Events
    void RoundOverEventListener(std::function<void()> callback);
    void RaiseRoundOverEvent();

    void RoundInformationChangedListener(std::function<void(Round*)> callback);
    void RaiseRoundInformationChangedEvent();
#pragma endregion
};

#endif