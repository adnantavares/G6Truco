#ifndef TRUCOCONTROLLER_H
#define TRUCOCONTROLLER_H
#include "Game.h"
class TrucoController {
private:
    Player player1, player2, player3, player4;
    Round round;
    std::function<void(Player*)> activePlayerChangedEvent;
    std::function<void(Round*)> roundInformationsChangedEvent;

public:
    TrucoController();
    void OnCardPlayed(int playerIndex, Card card); // Called when a card is played.
    void OnBetCalled(int playerIndex, int bet); // Called when a bet is made.
    void StartGame();

#pragma region Events
#pragma region Round events
    void ActivePlayerChangedEvent(std::function<void(Player*)> callback);
    void RaiseActivePlayerChangedEvent(Player* currentPlayer);
    void RoundInformationsChangedEvent(std::function<void(Round*)> callback);
    void RaiseRoundInformationsChangedEvent(Round* currentRoundInformations);
#pragma endregion

#pragma endregion

};
#endif //TRUCOCONTROLLER_H
