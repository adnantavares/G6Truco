#ifndef TRUCOCONTROLLER_H
#define TRUCOCONTROLLER_H
#include "Game.h"
#include "CardStrengthCalculator.h"
#include "HumanPlayer.h"
#include "CPUPlayer.h"

class TrucoController {
private:
    Round round;
    std::function<void(Player*)> activePlayerChangedEvent;
    std::function<void(Round*)> roundInformationsChangedEvent;

public:
    TrucoController();
    void PlayCard();
    void RaiseBet();
    void StartGame();
    bool IsActivePlayerHuman();
    bool TrySetSelectedCardIndex(int index);

#pragma region Events
#pragma region Round events
    void ActivePlayerChangedEvent(std::function<void(Player*)> callback);
    void RaiseActivePlayerChangedEvent(Player* currentPlayer);
    void RoundInformationsChangedEvent(std::function<void(Round*)> callback);
    void RaiseRoundInformationsChangedEvent(Round* currentRoundInformations);
    void HandleRoundOver();
#pragma endregion

#pragma endregion

};
#endif //TRUCOCONTROLLER_H
