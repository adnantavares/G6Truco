#pragma once
#include <algorithm>
#include <vector>
#include "Player.h"
#include "CardStrengthCalculator.h"

class CPUPlayer : public Player {
public:
    CPUPlayer();
    Card PlayCard() override;
};
