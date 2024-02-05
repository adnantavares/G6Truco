#ifndef TRUCOCONTROLLER_H
#define TRUCOCONTROLLER_H
#include "Game.h"
#include "CardStrengthCalculator.h"

class TrucoController {
private:
    Round round;
    std::function<void(Player*)> activePlayerChangedEvent;
    std::function<void(Round*)> roundInformationsChangedEvent;

public:
    TrucoController();
    void PlayCard(int cardIndex); // Called when a card is played.
    void RaiseBet(); // Called when a bet is made.
    void StartGame();

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
