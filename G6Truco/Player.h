#pragma once
#include <vector>
#include "Card.h"

class Player {
private:
    std::vector<Card> hand; // Cards in the player's hand.
    bool isHandHidden; // Indicates if the card is hidden.

public:
    void receiveHand(std::vector<Card>& newHand); // Receives the dealt cards.
    Card playCard(size_t cardIndex, bool hideCard); // Plays a card.
    void viewHand(); // Allows viewing the hand in Hand of Eleven.
};
