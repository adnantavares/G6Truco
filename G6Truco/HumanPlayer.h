#pragma once
#include "Player.h"
#include <memory>

class HumanPlayer : public Player {
private:
    HumanPlayer(const CString& name);

public:
    static std::unique_ptr<HumanPlayer> Create(const CString& name);
    Card PlayCard() override;
};
