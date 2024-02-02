#ifndef CARD_STRENGTH_CALCULATOR_H
#define CARD_STRENGTH_CALCULATOR_H

#include "Card.h"
#include <map>

// This class calculates the strength of cards in a Truco game, including determining manilhas.
class CardStrengthCalculator {
private:
    Card vira; // The reference card to determine manilhas
    std::map<Card::Rank, int> rankStrength; // Maps card ranks to their strengths
    std::map<Card::Suit, double> suitStrength; // Maps card suits to their strengths for manilhas

public:
    CardStrengthCalculator(Card viraCard); // Constructor takes the vira card
    void InitializeStrengthMaps(); // Initializes the strength values for ranks and suits
    Card::Rank GetManilhaRank() const; // Determines the rank of the manilha based on the vira
    double GetCardStrength(const Card& card) const; // Returns the strength of a given card
};

#endif // CARD_STRENGTH_CALCULATOR_H
