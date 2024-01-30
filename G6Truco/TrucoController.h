#ifndef TRUCOCONTROLLER_H
#define TRUCOCONTROLLER_H
#include "Game.h"
class TrucoController {
private:
    Player player1, player2, player3, player4;
    Round round;

public:
    TrucoController();
    void OnCardPlayed(int playerIndex, Card card); // Called when a card is played.
    void OnBetCalled(int playerIndex, int bet); // Called when a bet is made.
    void StartGame();
};
#endif //TRUCOCONTROLLER_H
