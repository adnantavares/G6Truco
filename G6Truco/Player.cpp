#include "pch.h"
#include "Player.h"

Player::Player() : isHandHidden(false)
{

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

std::vector<Card> Player::GetHand() const
{
    return hand;
}

void Player::SetPlayerName(const CString& name)
{
    playerName = name;
}

CString Player::GetPlayerName() const
{
    return playerName;
}
#pragma endregion

