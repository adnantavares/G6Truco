#ifndef DECK_H
#define DECK_H
#include <vector>
#include "Card.h"
#include <algorithm> 
#include <random>    
#include <chrono>    

class Deck {
private:
    std::vector<Card> cards; // Stores the deck of cards.

    bool IsEmpty() const;

public:
    Deck(); // Constructor to initialize the deck with 40 cards.
    void InitializeDeck();
    void Shuffle(); // Shuffles the cards.
    Card TakeTopCard();
};
#endif // DECK_H
