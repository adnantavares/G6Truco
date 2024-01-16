#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include "Game.h"
#include "G6TrucoView.h"
class GameController {
private:
    Game model; // The game model.
    CG6TrucoView view; // The game view.

public:
    void OnCardPlayed(int playerIndex, int cardIndex, bool hideCard); // Called when a card is played.
    void OnBetCalled(int playerIndex, int bet); // Called when a bet is made.
};
#endif //GAMECONTROLLER_H
