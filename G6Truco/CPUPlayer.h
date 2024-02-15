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
    Card PlaySelectedCard(const Card& cardToPlay);
    Card FindWeakestCard() const;
    std::optional<Card> FindStrongerCard(const Card& targetCard) const;
    void AnalyzeCurrentBet(Round* round);
    void TriggerPlayIntention(Round* round);
    void MonitorRoundState(Round* round);
    int DecideBetAcceptance(int currentBet);

public:
    static std::unique_ptr<CPUPlayer> Create(const CString& name, Round* round);
    static void NotifyPlayers(bool notifyAll = true);

    Card PlayCard() override;
    void SetWinningCard(std::optional<std::pair<Player*, Card>> newWinningCard);
    void SetCurrentViraCard(Card viraCard);
#pragma region Events
    void SetRaiseBetCallback(std::function<void(Player*, int)> callback);
    void RaiseBetEvent(int bet);
#pragma endregion
};
