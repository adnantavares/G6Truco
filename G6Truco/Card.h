#pragma once
class Card {
public:
    enum Suit { HEARTS, SPADES, DIAMONDS, CLUBS };
    enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, JACK, QUEEN, KING, ACE };

private:
    Suit suit;
    Rank rank;
};