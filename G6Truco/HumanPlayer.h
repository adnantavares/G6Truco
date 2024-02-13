#pragma once
#include "Player.h"
#include <memory>

class HumanPlayer : public Player {
private:
    int selectedCardIndex = -1;
    HumanPlayer(const CString& name);

public:
    static std::unique_ptr<HumanPlayer> Create(const CString& name);
    Card PlayCard() override;

#pragma region Getters and Setters
    void SetSelectCardIndex(int index);
    int GetSelectCardIndex() const;
#pragma endregion

};
