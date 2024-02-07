#pragma once
#include <algorithm>
#include <vector>
#include "Player.h"
#include "CardStrengthCalculator.h"

class CPUPlayer : public Player {
private:
    int selectedCardIndex = -1;
public:
    CPUPlayer();
    Card PlayCard() override;
};
