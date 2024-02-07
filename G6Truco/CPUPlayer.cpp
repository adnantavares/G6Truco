#include "pch.h"
#include "CPUPlayer.h"

CPUPlayer::CPUPlayer()
{
}

//TODO: Change this method in order to make a decision to select a card
Card CPUPlayer::PlayCard()
{
    Card cardToPlay = GetHand()[0];
    auto it = std::find_if(hand.begin(), hand.end(), [&cardToPlay](const Card& card) {
        return card.GetSuit() == cardToPlay.GetSuit() && card.GetRank() == cardToPlay.GetRank();
        });

    if (it != hand.end()) {
        hand.erase(it);
    }
    return cardToPlay;
}
