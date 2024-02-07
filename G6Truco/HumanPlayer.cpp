#include "pch.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer()
{
}

Card HumanPlayer::PlayCard()
{
    Card cardToPlay = GetHand()[selectedCardIndex];
    auto it = std::find_if(hand.begin(), hand.end(), [&cardToPlay](const Card& card) {
        return card.GetSuit() == cardToPlay.GetSuit() && card.GetRank() == cardToPlay.GetRank();
        });

    if (it != hand.end()) {
        hand.erase(it);
    }
    return cardToPlay;
}

#pragma region Getters and Setters
void HumanPlayer::SetSelectCardIndex(int index)
{
    selectedCardIndex = index;
}

int HumanPlayer::GetSelectCardIndex() const
{
    return selectedCardIndex;
}
#pragma endregion