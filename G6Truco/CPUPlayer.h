#pragma once
#include <algorithm>
#include <vector>
#include "Player.h"
#include "CardStrengthCalculator.h"
#include <optional>

class CPUPlayer : public Player {
private:
    std::optional<std::pair<Player*, Card>> winningCard;
    Card currentViraCard;
public:
    CPUPlayer();
    Card PlayCard() override;
    void SetWinningCard(std::optional<std::pair<Player*, Card>> newWinningCard);
    void SetCurrentViraCard(Card viraCard);

private:
    Card PlaySelectedCard(const Card& cardToPlay);
    Card CPUPlayer::FindWeakestCard() const;
    std::optional<Card> CPUPlayer::FindStrongerCard(const Card& targetCard) const;
};
