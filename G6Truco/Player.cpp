#include "pch.h"
#include "Player.h"

Player::Player()
{

}

bool Player::PlayCard(const Card& cardToPlay)
{
    auto it = std::find_if(hand.begin(), hand.end(), [&cardToPlay](const Card& card) {
        return card.GetSuit() == cardToPlay.GetSuit() && card.GetRank() == cardToPlay.GetRank();
    });

    if (it != hand.end()) {
        hand.erase(it);
        return true;
    }

    return false; 
}

#pragma region Events
void Player::SetRaiseBetCallback(std::function<void(Player*, int)> callback) {
    raiseBetCallback = callback;
}

void Player::RaiseBetEvent(int bet) {
    if (raiseBetCallback) {
        raiseBetCallback(this, bet);
    }
}
#pragma endregion

#pragma region Getters and Setters
void Player::SetHand(std::vector<Card>& newHand)
{
    hand = newHand;
}

std::vector<Card> Player::GetHand()
{
    return hand;
}

void Player::SetPlayerName(const CString& name)
{
    playerName = name;
}

CString Player::GetPlayerName()
{
    return playerName;
}
#pragma endregion

