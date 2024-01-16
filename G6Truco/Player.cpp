#include "pch.h"
#include "Player.h"

Player::Player()
{

}

void Player::SetRaiseBetCallback(std::function<void(Player*, int)> callback) {
    raiseBetCallback = callback;
}

void Player::RaiseBetEvent(int bet) {
    if (raiseBetCallback) {
        raiseBetCallback(this, bet);
    }
}
