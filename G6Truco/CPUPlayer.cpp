#include "pch.h"
#include "CPUPlayer.h"

CPUPlayer::CPUPlayer() : Player(PlayerType::Cpu)
{
}

Card CPUPlayer::PlayCard()
{
    // If no card is winning yet, play the weakest card
    if (!winningCard.has_value()) {
        return PlaySelectedCard(FindWeakestCard());
    }

    const auto& [winningPlayer, winningCardValue] = winningCard.value();

    // If the winning card is from the opposing team, try to play a stronger card
    if (winningPlayer->playerType == Player::PlayerType::Human) {
        auto strongerCard = FindStrongerCard(winningCardValue);
        if (strongerCard) {
            return PlaySelectedCard(*strongerCard);
        }
        else {
            // If any card is stronger than 'winningCard', play the weakest card
            return PlaySelectedCard(FindWeakestCard());
        }
    }
    else {
        // Play the weakest card, since the 'winningCard' is from another CPUPlayer
        return PlaySelectedCard(FindWeakestCard());
    }
}

Card CPUPlayer::PlaySelectedCard(const Card& cardToPlay) {
    auto it = std::find(hand.begin(), hand.end(), cardToPlay);
    if (it != hand.end()) {
        hand.erase(it);
    }
    return cardToPlay;
}

void CPUPlayer::SetWinningCard(std::optional<std::pair<Player*, Card>> newWinningCard)
{
    winningCard = newWinningCard;
}

void CPUPlayer::SetCurrentViraCard(Card viraCard)
{
    currentViraCard = viraCard;
}

Card CPUPlayer::FindWeakestCard() const
{
    auto it = std::min_element(hand.begin(), hand.end(), [this](const Card& a, const Card& b) {
        CardStrengthCalculator strengthCalculator(currentViraCard);
        return strengthCalculator.GetCardStrength(a) < strengthCalculator.GetCardStrength(b);
        });
    return *it;
}

std::optional<Card> CPUPlayer::FindStrongerCard(const Card& targetCard) const
{
    CardStrengthCalculator strengthCalculator(currentViraCard);
    auto it = std::find_if(hand.begin(), hand.end(), [&](const Card& card) {
        return strengthCalculator.GetCardStrength(card) > strengthCalculator.GetCardStrength(targetCard);
        });

    if (it != hand.end()) {
        return *it;
    }
    else {
        return std::nullopt; // It don't have any stronger card
    }
}
