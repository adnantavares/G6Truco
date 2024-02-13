#include "pch.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const CString& name) : Player(PlayerType::Human)
{
    SetPlayerName(name);
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

std::unique_ptr<HumanPlayer> HumanPlayer::Create(const CString& name)
{
    return std::unique_ptr<HumanPlayer>(new HumanPlayer(name));
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