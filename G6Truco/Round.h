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
    std::optional<std::pair<Player*, Card>> winningCard; // The stronghest card in current round
    int currentBet; // The current bet (1, 3, 6, 9, 12).
    int activePlayerIndex;
    bool isRoundOver;
    std::function<void(Round*)> roundInformationChangedEvent;
    std::function<void()> roundOverEvent;
    std::vector<int> possibleBets;
    int currentTrucoCall;
    
    void PreviousBet(); // Find the previous bet considering its current value
    void NextBet(); // Find the next bet considering its current value
    void RemovePlayedCards(); // Clear the cards in table and remove them from the players hands
    void StartPlayingThread(); // Starts the thread that controls the playing execution flow of the round
    void DetermineRoundPoint(int& playerIndex); // Determines the winner of the round point
    void PlayCard(); // Method which calls playing action from players (Humans or CPU)
    void NextPlayer(); // Check all the conditions to determine next player on terminate the round
    void DealCardsToPlayers(); // Deals 3 new cards to the players
    std::vector<Card> TakeCardFromTopDeck(int numberOfCards); // Take card from the top of the deck (useful to set Vira card)
    bool IsNewCardStronger(const Card& newCard, const Card& currentWinningCard); // Checks if latest played card is stronger than the current winning card
    void OnRaiseBet(Player* player, int betDecision); // Event handler executed to receive Truco response from CPU Player thread
    bool IsHumanPlayer(Player* player); // Checks if a given player is human
    void DefineWinningCard(Card playedCard); // Check if latest card played is the strongest one and stores it

#pragma region Getters and Setters
    void SetViraCard(Card viraCard);
    void SetCurrentBet(int bet);
#pragma endregion

public:
    enum TrucoCallType { NONE, CPU_ACCEPTED, CPU_RAISED };

    Round();

    void NotifyPlayingAction(); // Wakes round playing thread up (through condition variable) in order to make players take action and set next player or finish the round
    void RaiseBet(); // Raises the current bet (Truco, Six, Nine, Twelve).
    void DenyBet(Player* player = nullptr); // Denies the current bet (leave the round)
    void StartRound(int firstPlayer); // Set/Reset basic attributes to start a new round
    bool IsRoundOver(); // Checks if a round is over considering the round points from each team
    bool IsHumanPlayer(); // Checks if the active player is human

#pragma region Getters and Setters
    int GetActivePlayerIndex() const;
    Player* GetActivePlayer();
    Card GetViraCard();
    const std::optional<std::pair<Player*, Card>> GetWinningCard();
    const std::array<std::unique_ptr<Player>, 4>& Round::GetAllPlayers() const;
    void SetPlayers(std::array<std::unique_ptr<Player>, 4>&& allPlayers);
    int GetCurrentBet();
    int GetWinnerTeam() const;
    int GetCurrentTrucoCall();
    void SetCurrentTrucoCall(int type);
    std::vector<int> GetPoints();
    CPUPlayer* GetCPUActivePlayer(); // Remark: nullPtr is returned if the active player is not a CPUPlayer
#pragma endregion

#pragma region Events
    void RoundOverEventListener(std::function<void()> callback);
    void RaiseRoundOverEvent();

    void RoundInformationChangedListener(std::function<void(Round*)> callback);
    void RaiseRoundInformationChangedEvent();
#pragma endregion
};

#endif