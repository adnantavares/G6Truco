#include "pch.h"
#include "CardStrengthCalculator.h"
#include <vector>
#include <algorithm>

CardStrengthCalculator::CardStrengthCalculator(Card viraCard) : vira(viraCard) {
    InitializeStrengthMaps();
}

void CardStrengthCalculator::InitializeStrengthMaps() {
    // Initialize the strength of suits
    suitStrength[Card::Suit::CLUBS] = 0.4;
    suitStrength[Card::Suit::HEARTS] = 0.3;
    suitStrength[Card::Suit::SPADES] = 0.2;
    suitStrength[Card::Suit::DIAMONDS] = 0.1;

    // Initialize the strength of ranks
    std::vector<Card::Rank> ranks = {
        Card::Rank::FOUR, Card::Rank::FIVE, Card::Rank::SIX, Card::Rank::SEVEN,
        Card::Rank::JACK, Card::Rank::QUEEN, Card::Rank::KING,
        Card::Rank::ACE, Card::Rank::TWO, Card::Rank::THREE
    };

    int strength = 1;
    for (auto& rank : ranks) {
        rankStrength[rank] = strength++;
    }
}

Card::Rank CardStrengthCalculator::GetManilhaRank() const {
    // Automatically determines the next rank as manilha, cycling after the ACE to TWO and SEVEN to JACK
    int nextRank = static_cast<int>(vira.GetRank()) + 1;
    if (vira.GetRank() == Card::Rank::SEVEN) {
        nextRank = static_cast<int>(Card::Rank::JACK);
    }
    else if (vira.GetRank() == Card::Rank::ACE) {
        nextRank = static_cast<int>(Card::Rank::TWO);
    }
    return static_cast<Card::Rank>(nextRank);
}

double CardStrengthCalculator::GetCardStrength(const Card& card) const {
    // Manilhas have a base strength of 1000, plus the suit strength
    if (card.GetRank() == GetManilhaRank()) {
        return 1000 + suitStrength.at(card.GetSuit());
    }
    // Other cards' strength is determined by their rank, plus a minor adjustment for the suit
    return rankStrength.at(card.GetRank()) + suitStrength.at(card.GetSuit());
}
