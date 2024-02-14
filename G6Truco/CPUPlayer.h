#pragma once
#include <algorithm>
#include <vector>
#include "Player.h"
#include "Round.h"
#include "CardStrengthCalculator.h"
#include <optional>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

// Forward Declaration
class Round;


class CPUPlayer : public Player {
private:
    static std::mutex playerMutex;
    static std::condition_variable playerConditionVariable;

    const int PLAY_INTERVAL = 3000;

    std::optional<std::pair<Player*, Card>> winningCard;
    Card currentViraCard;
    CPUPlayer(const CString& name);

public:
    static std::unique_ptr<CPUPlayer> Create(const CString& name, Round* round);
    static void NotifyPlayers();

    Card PlayCard() override;
    void SetWinningCard(std::optional<std::pair<Player*, Card>> newWinningCard);
    void SetCurrentViraCard(Card viraCard);

private:
    Card PlaySelectedCard(const Card& cardToPlay);
    Card CPUPlayer::FindWeakestCard() const;
    std::optional<Card> CPUPlayer::FindStrongerCard(const Card& targetCard) const;
    void MonitorRoundState(Round* round);
};
