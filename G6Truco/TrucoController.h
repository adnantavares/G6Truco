#ifndef TRUCOCONTROLLER_H
#define TRUCOCONTROLLER_H
#include "Game.h"
#include "CardStrengthCalculator.h"
#include "HumanPlayer.h"
#include "CPUPlayer.h"

class TrucoController {
private:
    const int NUMBER_OF_PLAYERS = 4;
    int firstPlayer;
    std::unique_ptr<Round> round;
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
    void ActivePlayerChangedEventListener(std::function<void(Player*)> callback);
    void RaiseActivePlayerChangedEvent(Player* currentPlayer);
    void RoundInformationChangedEventListener(std::function<void(Round*)> callback);
    void RaiseRoundInformationChangedEvent(Round* currentRoundInformations);
    void HandleRoundOver();
#pragma endregion

#pragma endregion

};
#endif //TRUCOCONTROLLER_H
