#include "pch.h"
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(const CString& name) : Player(PlayerType::Human)
{
    SetPlayerName(name);
}

Card HumanPlayer::PlayCard()
{
    return GetHand()[selectedCardIndex];
}

std::unique_ptr<HumanPlayer> HumanPlayer::Create(const CString& name)
{
    return std::unique_ptr<HumanPlayer>(new HumanPlayer(name));
}

