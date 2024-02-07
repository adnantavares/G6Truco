#pragma once
#include "Player.h"

class HumanPlayer : public Player {
private:
    int selectedCardIndex = -1;
public:
    HumanPlayer();
    Card PlayCard() override;

#pragma region Getters and Setters
    void SetSelectCardIndex(int index);
    int GetSelectCardIndex() const;
#pragma endregion

};
