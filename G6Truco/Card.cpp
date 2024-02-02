#include "pch.h"
#include "Card.h"

Card::Card() {
}

Card::Card(Suit s, Rank r) : suit(s), rank(r) {
}

#pragma region Getters and Setters
Card::Suit Card::GetSuit() const {
    return suit;
}

Card::Rank Card::GetRank() const {
    return rank;
}
#pragma endregion
