#include "pch.h"
#include "Deck.h"

Deck::Deck() {
    InitializeDeck();
}

void Deck::InitializeDeck() {
    cards.clear();

    for (int suit = Card::HEARTS; suit <= Card::CLUBS; ++suit) {
        for (int rank = Card::TWO; rank <= Card::ACE; ++rank) {
            // excluding invalid cards from Truco game
            //if (rank != Card::EIGHT && rank != Card::NINE && rank != Card::TEN) {
                cards.emplace_back(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank));
            //}
        }
    }
}

void Deck::Shuffle()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::default_random_engine engine(seed);
    std::shuffle(cards.begin(), cards.end(), engine);
}

bool Deck::IsEmpty() const {
    return cards.empty();
}

Card Deck::TakeTopCard() {
    if (IsEmpty())
    {
        InitializeDeck();
        Shuffle();
    }
    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}
