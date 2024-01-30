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

public:
    Deck(); // Constructor to initialize the deck with 40 cards.
    void InitializeDeck();
    void Shuffle(); // Shuffles the cards.
    std::vector<Card> DealHand(); // Deals a hand of 3 cards.
    Card FlipVira(); // Flips the card that defines the manilhas.
};
#endif // DECK_H
