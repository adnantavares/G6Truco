#ifndef TRUCOCONTROLLER_H
#define TRUCOCONTROLLER_H
#include "Game.h"
#include "CardStrengthCalculator.h"
#include "HumanPlayer.h"
#include "CPUPlayer.h"

class TrucoController {
private:
    std::mutex roundMutex;
    std::condition_variable roundConditionVariable;
    std::thread roundHandlerThread;

    int firstPlayer;
    std::unique_ptr<Round> round;
    std::function<void(Player*)> activePlayerChangedEvent;
    std::array<int, 2> gamePoints;

    void StartRoundHandlerThread();
    void NotifyRoundOver();
    bool IsActivePlayerHuman();

#pragma region Events Triggers
    void RaiseActivePlayerChangedEvent(Player* currentPlayer);
#pragma endregion

public:
    TrucoController();
    void PlayCard();
    void RaiseBet();
    void DenyBet();
    void AcceptBet();
    void StartGame();
    bool TrySetSelectedCardIndex(int index);
    std::array<int, 2> GetGamePoints();

#pragma region Event listeners
    void ActivePlayerChangedEventListener(std::function<void(Player*)> callback);
    void RoundInformationChangedEventListener(std::function<void(Round*)> callback);
#pragma endregion
};
#endif //TRUCOCONTROLLER_H