#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include <functional>

class Player {
private:
    std::vector<Card> hand; // Cards in the player's hand.
    bool isHandHidden; // Indicates if the card is hidden.
    std::function<void(Player*, int)> raiseBetCallback;
    CString playerName;

public:
    Player();
    bool PlayCard(const Card& cardToPlay);
#pragma region Getters and Setters
    void SetHand(std::vector<Card>& newHand);
    std::vector<Card> GetHand();
    void SetPlayerName(const CString& name);
    CString GetPlayerName();
#pragma endregion

#pragma region Events
    void SetRaiseBetCallback(std::function<void(Player*, int)> callback);
    void RaiseBetEvent(int bet);
#pragma endregion

};

#endif // PLAYER_H
