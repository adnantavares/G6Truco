#pragma once
#include <algorithm>
#include <vector>
#include "Player.h"
#include "Round.h"
#include "CardStrengthCalculator.h"
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>

// Forward Declaration
class Round;

// This class 
class CPUPlayer : public Player {
private:
    const int PLAY_INTERVAL = 3000;

    static std::mutex playerMutex;
    static std::condition_variable playerConditionVariable;
    static int lastCurrentBet;

    Card currentViraCard;
    std::optional<std::pair<Player*, Card>> winningCard;
    std::function<void(Player*, int)> raiseBetCallback;

    CPUPlayer(const CString& name); 
    Card PlaySelectedCard(const Card& cardToPlay);  // Finds selected card between the ones in hand and store its index.
    Card FindWeakestCard() const; // Finds the card which has the smallest value in hand
    std::optional<Card> FindStrongerCard(const Card& targetCard) const; // Finds the card (in hand) which has a greater value than the target one
    void AnalyzeCurrentBet(Round* round); // Checks the current bet from the round to take an action
    void TriggerPlayIntention(Round* round); // Sends signal to the Round Playing thread
    void MonitorRoundState(Round* round); // Starts thread that monitors round state always a condition variable is notified.
    int DecideBetAcceptance(Round* round); // Decides for accepting, raising or denying a bet (Truco)
    void SetWinningCard(std::optional<std::pair<Player*, Card>> newWinningCard); // Receives winning card from the round to be used in playing decision

public:
    static std::unique_ptr<CPUPlayer> Create(const CString& name, Round* round); // Factor method to properly initialize a player
    static void NotifyPlayers(bool notifyAll = true); // Wakes player threads up (through condition variable) in order to make them analyze bets and trigger playing intention
    static void ResetPlayerBet(); // Resets last bet registered by the player threads

    Card PlayCard() override; // Plays "best" card from the hand, according to the situation in game
    void SetCurrentViraCard(Card viraCard); // Stores vira card to calculate values right before playing 
#pragma region Events
    void SetRaiseBetCallback(std::function<void(Player*, int)> callback); // Set callback to raise bet event
    void RaiseBetEvent(int bet); // Event triggered to inform the CPU Player decision about a bet proposal (Truco)
#pragma endregion
};
