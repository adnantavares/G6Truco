#include "pch.h"
#include "Player.h"

Player::Player()
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

