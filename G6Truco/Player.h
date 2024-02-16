#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include <functional>

class Player {
public:
    enum class PlayerType { Human, Cpu };
protected:
    std::vector<Card> hand;
    bool isHandHidden;
    CString playerName;
    int selectedCardIndex = -1;

    void SetPlayerName(const CString& name);

public:
    Player(PlayerType type);
    ~Player() = default;
    PlayerType playerType;

    void RemoveSelectedCard();
    virtual Card PlayCard() = 0;
    
#pragma region Getters and Setters
    void SetHand(std::vector<Card>& newHand);
    std::vector<Card> GetHand() const;
    CString GetPlayerName() const;
    void SetSelectCardIndex(int index);
    int GetSelectCardIndex() const;
#pragma endregion
};

#endif // PLAYER_H
