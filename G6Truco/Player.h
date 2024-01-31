#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"
#include <functional>

class Player {
private:
    std::vector<Card> hand; // Cards in the player's hand.
    bool isHandHidden; // Indicates if the card is hidden.
    std::function<void(Player*, int)> raiseBetCallback; // Event

public:
    Player();
    void ReceiveHand(std::vector<Card>& newHand); // Receives the dealt cards.
    Card PlayCard(size_t cardIndex, bool hideCard); // Plays a card.
    void ViewHand(); // Allows viewing the hand in Hand of Eleven.
    void SetRaiseBetCallback(std::function<void(Player*, int)> callback);
    void RaiseBetEvent(int bet);
};

#endif // PLAYER_H
