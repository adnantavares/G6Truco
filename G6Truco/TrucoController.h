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

    void RaiseActivePlayerChangedEvent(Player* currentPlayer);
    void HandleRoundOver();

public:
    TrucoController();
    void PlayCard();
    void RaiseBet();
    void StartGame();
    bool IsActivePlayerHuman();
    bool TrySetSelectedCardIndex(int index);

#pragma region Events
#pragma region Round event listeners
    void ActivePlayerChangedEventListener(std::function<void(Player*)> callback);
    void RoundInformationChangedEventListener(std::function<void(Round*)> callback);
#pragma endregion

#pragma endregion

};
#endif //TRUCOCONTROLLER_H
