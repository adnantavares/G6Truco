#pragma once
#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> cards; // Stores the deck of cards.

public:
    Deck(); // Constructor to initialize the deck with 40 cards.
    void shuffle(); // Shuffles the cards.
    std::vector<Card> dealHand(); // Deals a hand of 3 cards.
    Card flipVira(); // Flips the card that defines the manilhas.
};

