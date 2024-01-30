#ifndef CARD_H
#define CARD_H

class Card {
public:
    enum Suit { HEARTS, SPADES, DIAMONDS, CLUBS };
    enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, JACK, QUEEN, KING, ACE };

private:
    Suit suit;
    Rank rank;
};
#endif //CARD_H