#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include <functional>

class Player {
protected:
    std::vector<Card> hand;
    bool isHandHidden;
    std::function<void(Player*, int)> raiseBetCallback;
    CString playerName;

public:
    Player();
    ~Player() = default;

    virtual Card PlayCard() = 0;

#pragma region Getters and Setters
    void SetHand(std::vector<Card>& newHand);
    std::vector<Card> GetHand() const;
    void SetPlayerName(const CString& name);
    CString GetPlayerName() const;
#pragma endregion

#pragma region Events
    void SetRaiseBetCallback(std::function<void(Player*, int)> callback);
    void RaiseBetEvent(int bet);
#pragma endregion
};

#endif // PLAYER_H
